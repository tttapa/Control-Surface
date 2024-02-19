#include "BulkTX.hpp"

#include <AH/Arduino-Wrapper.h>
#include <AH/Containers/CRTP.hpp>

#include <algorithm>

#include <cassert>

#ifdef FATAL_ERRORS
#define CS_MIDI_USB_ASSERT(a) assert((a))
#else
#define CS_MIDI_USB_ASSERT(a)
#endif

BEGIN_CS_NAMESPACE

template <class Derived, class MessageTypeT, uint16_t PacketSizeV>
void BulkTX<Derived, MessageTypeT, PacketSizeV>::write(MessageType msg) {
    write_impl(&msg, 1, false); // blocking
}

template <class Derived, class MessageTypeT, uint16_t PacketSizeV>
void BulkTX<Derived, MessageTypeT, PacketSizeV>::write(const MessageType *msgs,
                                                       uint32_t num_msgs) {
    const uint32_t *end = msgs + num_msgs;
    while (msgs != end) msgs += write_impl(msgs, end - msgs, false); // blocking
}

template <class Derived, class MessageTypeT, uint16_t PacketSizeV>
uint32_t BulkTX<Derived, MessageTypeT, PacketSizeV>::write_nonblock(
    const MessageType *msgs, uint32_t num_msgs) {
    uint32_t total_sent = 0, sent = 1;
    while (total_sent < num_msgs && sent != 0) {
        sent = write_impl(msgs + total_sent, num_msgs - total_sent, true);
        total_sent += sent;
    }
    return total_sent;
}

template <class Derived, class MessageTypeT, uint16_t PacketSizeV>
void BulkTX<Derived, MessageTypeT, PacketSizeV>::send_now() {
    uint32_t size;
    wbuffer_t *writebuf;
    std::tie(writebuf, size) = read_writebuf_size();
    if (size > 0)
        send_now_impl_nonblock(writebuf);
}

/*
 * write(...)  
 *  - called by user
 *  - adds data to the transmit buffer
 *  - starts a timeout to send the data (even if the buffer isn't full yet)
 *  - sends the transmit buffer to the USB stack when full
 *  - blocks if there's no space left in the transmit buffers
 * 
 * timeout_callback()  
 *  - called from a timer interrupt, a fixed time after the first write to a
 *    buffer
 *  - sends the transmit buffer even if it isn't full yet
 *  - if a previous transmission is still in progress, schedules it to be sent
 *    immediately after the current transmission finishes.
 * 
 * in_callback()  
 *  - called by the USB stack when a transmission is complete
 *  - finishes the transmission
 *  - releases the “sending” lock
 *  - starts a new transmission if it was scheduled while the previous 
 *    transmission was in progress
 */

/*
 * - Both send buffers start out empty (size = 0), the first buffer is active 
 *   (`active_writebuffer = buffers[0]`, which means that the first buffer is
 *   the one being filled with the outgoing data).
 * - The `write()` function is called.
 * - The message is added to the active buffer and the size is incremented. 
 *   (Writing the data happens inside of an atomic compare-and-swap loop, if in
 *   the meantime an interrupts occurs that sends the buffer we just wrote to,
 *   we write the data again to the new writing buffer.)
 * - If the active buffer is now full, the packet is sent to the USB stack, and
 *   the buffers are swapped. If the previous buffer wasn't completely sent yet,
 *   it is scheduled to be sent when the previous transmission is complete.
 * - If this was the first message in the buffer, a timeout is started.
 * - If the active buffer wasn't full, nothing is actually sent, and the 
 *   `write()` function exits.
 * - When the timeout fires, `timeout_callback()` is called (from a timer
 *   interrupt).
 * - If there is data in the active buffer, it's sent over USB. If the previous
 *   buffer wasn't completely sent yet, it's not possible to send now, and it's
 *   scheduled to be sent later.
 * - When a buffer has been sent, `in_callback()` is called (from a USB 
 *   interrupt).
 * - The write is finished, the “sending” lock is released and the buffer size
 *   is reset to zero to let the main `write()` function know that it can write 
 *   to this buffer. If the other buffer was scheduled to be sent while we were
 *   sending this packet, immediately send the next one.
 */

/*
 * `active_writebuffer` points to the buffer that the main program should write
 * to. It is possible that while writing data, the active buffer is sent from an
 * interrupt handler. This is not an issue, because the data is only published
 * by store-releasing the `size` of the buffer. This happens in a compare-
 * exchange loop: before sending a buffer, the sender sets the size to
 * `Reserved`, which will cause the compare-exchange in the `write_impl`
 * function to fail, so it can then write the data again to the new active
 * buffer.
 * 
 * `active_writebuffer` is only written to while holding the `sending` lock,
 * during the preparation of actually sending the previously active buffer.
 * After writing `active_writebuffer`, the size of the previously active buffer
 * is updated: first it is set to `Reserved`, and later, it is set to `0` (after
 * the buffer has been sent), using a store-release to `size`.
 * 
 * Therefore, the main program should first do a relaxed load of
 * `active_writebuffer`, and then a load-acquire of `active_writebuffer->size`.
 * If this size is either `Reserved` or `0`, it should load `active_writebuffer`
 * again, and then load-acquire the size again. This newly read size is then
 * guaranteed to be zero, and `active_writebuffer` points to an empty buffer
 * that is not yet scheduled to be sent. Because of the load-acquire of the
 * `size`, it automatically synchronizes with the sending of the data, ensuring
 * that the buffer is ready to use.
 * 
 * When the first data is added to an empty buffer, a pointer to it is stored in
 * `send_later`, and a timeout is started.
 * 
 * There are two events that can cause the `send_later` buffer to be sent:
 * 1. The main program calls `send_now_impl_nonblock` (either because the user
 *   called `send_now`, or because the user called `write` and the buffer was
 * full);
 * 2. The timeout fires.
 * 
 * Only one of the two can actually send it, because they both try to acquire
 * the `send_later` buffer. The other one gives up, so they don't run
 * concurrently.
 * If `send_now_impl_nonblock` acquires the `send_later` buffer, it cancels the
 * timer because it is no longer necessary.
 * 
 * Whichever function acquires the `send_later` buffer, it makes sure that makes
 * progress is made: either it sends the buffer immediately, or if the previous
 * transfer is still in progress, it schedules it to be sent as soon as that one
 * finishes.  
 * This is done by first setting the `send_now` buffer to the one it acquired
 * from `send_later`. Then, it tries to acquire the `sending` lock. If this
 * fails, it returns, because the `in_handler` is guaranteed to send the
 * `send_later` buffer.
 * If acquiring the `sending` lock succeeds, `send_now` is acquired and cleared,
 * and if it is not null, the active buffer is swapped, the old buffer's size is
 * set to `Reserved`, and a USB transfer is started.
 */

template <class Derived, class MessageTypeT, uint16_t PacketSizeV>
void BulkTX<Derived, MessageTypeT, PacketSizeV>::reset() {
    writing.buffers[0].size.store(0, mo_rlx);
    writing.buffers[1].size.store(0, mo_rlx);
    writing.active_writebuffer.store(&writing.buffers[0], mo_rlx);
    writing.send_later.store(nullptr, mo_rlx);
    writing.send_now.store(nullptr, mo_rlx);
    writing.sending.store(nullptr, mo_rlx);
}

template <class Derived, class MessageTypeT, uint16_t PacketSizeV>
auto BulkTX<Derived, MessageTypeT, PacketSizeV>::read_writebuf_size()
    -> std::tuple<wbuffer_t *, uint16_t> {
    wbuffer_t *writebuffer;
    uint16_t size;
    writebuffer = writing.active_writebuffer.load(mo_rlx);
    size = writebuffer->size.load(mo_acq);
    // If the size was set to reserved, the active buffer must have been swapped.
    if (size == SizeReserved || size == 0) {
        [[maybe_unused]] wbuffer_t *old_buffer = writebuffer;
        writebuffer = writing.active_writebuffer.load(mo_rlx);
        CS_MIDI_USB_ASSERT(size == 0 || old_buffer != writebuffer);
        size = writebuffer->size.load(mo_acq);
    }
    CS_MIDI_USB_ASSERT(size != SizeReserved);
    return std::make_tuple(writebuffer, size);
}

template <class Derived, class MessageTypeT, uint16_t PacketSizeV>
uint32_t BulkTX<Derived, MessageTypeT, PacketSizeV>::write_impl(
    const MessageType *msgs, uint32_t num_msgs, bool nonblocking) {
    wbuffer_t *writebuf; // Pointer to buffer currently being written to.
    uint16_t size;       // Current size of that buffer (i.e. the first index we
                         // can write to).
    // TODO: move to wait_for_buffer helper function
    // Read the active write buffer and its current size
    std::tie(writebuf, size) = read_writebuf_size();
    // Make sure that there's space in the buffer for us to write data
    if (size >= PacketSize) {
        // If there's no space
        if (nonblocking) {
            // either return without blocking
            return 0;
        } else {
            // Or wait until the active writing buffer changes to an empty one
            [[maybe_unused]] wbuffer_t *old_buffer = writebuf;
            do {
                yield(); // spin
                std::tie(writebuf, size) = read_writebuf_size();
            } while (size >= PacketSize);
            CS_MIDI_USB_ASSERT(old_buffer != writebuf);
            CS_MIDI_USB_ASSERT(size == 0);
        }
    }
    CS_MIDI_USB_ASSERT(size < PacketSize);

    // Copy the data into the active buffer
    constexpr static uint32_t msg_size = sizeof(MessageType);
    static_assert(PacketSize % msg_size == 0, "");
    uint16_t free_size =
        std::min<uint32_t>(PacketSize - size, num_msgs * msg_size);
    CS_MIDI_USB_ASSERT(free_size % msg_size == 0);
    memcpy(&writebuf->buffer[size], msgs, free_size);
    uint16_t newsize = size + free_size;

    // Update the size of the buffer
    while (!writebuf->size.compare_exchange_weak(size, newsize, mo_rel)) {
        // If the size changed while we were writing the data, this means
        // that a sender in a different thread or ISR was/is sending the active
        // buffer (so it is no longer the active buffer).

        // Read the new write buffer and its size.
        // The cmp_xchg does not perform an acquire, because we'll/ acquire the
        // size here:
        std::tie(writebuf, size) = read_writebuf_size();
        // CS_MIDI_USB_ASSERT(size == 0); // Only true with cmp_xchg_strong

        // Copy the data into the new buffer
        free_size = std::min<uint32_t>(PacketSize - size, num_msgs * msg_size);
        memcpy(&writebuf->buffer[size], msgs, free_size);
        newsize = size + free_size;
    }

    // If this is the first data that's added to the buffer, indicate that it
    // can be sent later
    if (size == 0) {
        [[maybe_unused]] auto old =
            writing.send_later.exchange(writebuf, mo_rel);
        CS_MIDI_USB_ASSERT(old == nullptr);
    }
    // If the buffer is now full, send it immediately (but don't block)
    if (newsize == PacketSize) {
        send_now_impl_nonblock(writebuf);
    }
    // If this is the first data in the buffer, start a timer that will send
    // the buffer after a given timeout
    else if (size == 0) {
        std::atomic_signal_fence(mo_rel);
        CRTP(Derived).start_timeout();
    }

    // How many messages were added to the transmit buffer
    return free_size / msg_size;
}

template <class Derived, class MessageTypeT, uint16_t PacketSizeV>
void BulkTX<Derived, MessageTypeT, PacketSizeV>::send_now_impl_nonblock(
    wbuffer_t *send_buffer) {
    // Disable the timeout
    auto old = writing.send_later.exchange(nullptr, mo_acq);
    if (old == nullptr)
        // If the timeout was not active, the buffer has already been sent.
        return;

    // Cancel the timeout
    CS_MIDI_USB_ASSERT(old == send_buffer);
    CRTP(Derived).cancel_timeout();
    // Since we've cleared send_later, we can be sure that we're not racing
    // with the timeout handler

    // Schedule the buffer to be sent and reserve it (acquires the “sending”
    // lock on success, schedules the buffer to be sent later otherwise)
    auto size = prepare_tx(send_buffer);
    if (size == 0)
        return;

    // Actually send the buffer
    std::atomic_signal_fence(mo_rel);
    CRTP(Derived).tx_start(send_buffer->buffer, size);
    // “sending” lock is released in USB callback
    return;
}

template <class Derived, class MessageTypeT, uint16_t PacketSizeV>
void BulkTX<Derived, MessageTypeT, PacketSizeV>::timeout_callback() {
    // Check which buffer (if any) to send
    wbuffer_t *send_buffer = writing.send_later.exchange(nullptr, mo_acq);
    if (!send_buffer)
        // If the buffer has been cleared already, this means that the send_now
        // function was called before the timeout, and the buffer has been sent
        // (or marked ready), and we don't have to do anything
        return;

    // Schedule the buffer to be sent and reserve it (acquires the “sending”
    // lock on success, schedules the buffer to be sent later otherwise)
    auto size = prepare_tx(send_buffer);
    if (size == 0)
        return;

    // Actually send the buffer
    std::atomic_signal_fence(mo_rel);
    CRTP(Derived).tx_start_timeout(send_buffer->buffer, size);
}

template <class Derived, class MessageTypeT, uint16_t PacketSizeV>
uint16_t
BulkTX<Derived, MessageTypeT, PacketSizeV>::prepare_tx(wbuffer_t *send_buffer) {
    // Schedule the active buffer to be sent immediately after the previous one
    [[maybe_unused]] auto old = writing.send_now.exchange(send_buffer, mo_seq);
    // The send_later buffer can only be acquired once, so the send_now flag
    // cannot still be set
    CS_MIDI_USB_ASSERT(old == nullptr);

    // Try to acquire the “sending” lock:
    wbuffer_t *expected = nullptr;
    if (!writing.sending.compare_exchange_strong(expected, send_buffer, mo_seq))
        // If we failed to get the lock, the previous transmission is still in
        // progress, so we can't send now, but sending of our buffer is
        // scheduled using the send_now flag, so we can just return.
        return 0;

    // Acquire the send_now lock again (it might have been acquired by the
    // tx_callback in the meantime, in which case our job is done)
    if (writing.send_now.exchange(nullptr, mo_seq) == nullptr) {
        // Release the sending lock
        writing.sending.store(nullptr, mo_seq);
        return 0;
    }

    // Swap the buffers
    wbuffer_t *next_buffer = other_buf(send_buffer);
    // Since we were able to acquire the “sending” lock, the next buffer must be
    // empty.
    CS_MIDI_USB_ASSERT(next_buffer->size.load(mo_rlx) == 0);
    writing.active_writebuffer.store(next_buffer, mo_rlx);

    // Get the size of the buffer to send and atomically set it to reserved
    auto size = send_buffer->size.exchange(SizeReserved, mo_acq_rel);
    CS_MIDI_USB_ASSERT(size != SizeReserved);
    CS_MIDI_USB_ASSERT(size <= PacketSize);
    CS_MIDI_USB_ASSERT(size != 0);

    // Return the number of bytes that should be sent
    return size;
}

template <class Derived, class MessageTypeT, uint16_t PacketSizeV>
void BulkTX<Derived, MessageTypeT, PacketSizeV>::tx_callback() {
    wbuffer_t *sent_buffer = writing.sending.load(mo_acq);

    // The transmission is done, reset the size of the buffer to 0 so the
    // main thread can write to it again.
    [[maybe_unused]] auto oldsize = sent_buffer->size.exchange(0, mo_rel);
    CS_MIDI_USB_ASSERT(oldsize == SizeReserved);

    // Release the “sending” lock
    writing.sending.store(nullptr, mo_seq);

    // Check if the other buffer was scheduled to be sent immediately after the
    // one we just sent
    wbuffer_t *next_ready = writing.send_now.load(mo_seq);
    if (!next_ready)
        return;

    // Try to acquire the “sending” lock again
    wbuffer_t *expected = nullptr;
    if (!writing.sending.compare_exchange_strong(expected, next_ready, mo_seq))
        return;

    // Try to acquire the “send_now” buffer
    wbuffer_t *old_ready = writing.send_now.exchange(nullptr, mo_seq);
    if (!old_ready) {
        // If that failed, release the “sending” lock
        writing.sending.store(nullptr, mo_rel);
        return;
    }
    CS_MIDI_USB_ASSERT(old_ready == next_ready);

    // Send the next buffer if it was ready and if we were able to acquire the
    // sending lock

    // Swap the buffers
    writing.active_writebuffer.store(sent_buffer, mo_rlx);
    // Get the size of the buffer to send and atomically set it to reserved
    auto size = next_ready->size.exchange(SizeReserved, mo_acq_rel);
    CS_MIDI_USB_ASSERT(size != SizeReserved);
    CS_MIDI_USB_ASSERT(size != 0);
    // Actually send the buffer
    CRTP(Derived).tx_start_isr(next_ready->buffer, size);
    // Note that the timeout for this buffer cannot still be running, because
    // send_now was set, and this can only be done after clearing the timeout
    // (either in the timeout callback or in the send_now function)
}

END_CS_NAMESPACE

#undef CS_MIDI_USB_ASSERT
