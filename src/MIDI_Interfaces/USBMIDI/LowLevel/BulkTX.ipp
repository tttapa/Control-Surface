#include "BulkTX.hpp"

#include <AH/Arduino-Wrapper.h>
#include <AH/Containers/CRTP.hpp>

#include <algorithm>

#include <cassert>
#include <cstring>

#ifdef FATAL_ERRORS
#define CS_MIDI_USB_ASSERT(a) assert((a))
#else
#define CS_MIDI_USB_ASSERT(a)
#endif

BEGIN_CS_NAMESPACE

template <class Derived, class MessageTypeT, uint16_t MaxPacketSizeV>
void BulkTX<Derived, MessageTypeT, MaxPacketSizeV>::write(MessageType msg) {
    write(&msg, 1);
}

template <class Derived, class MessageTypeT, uint16_t MaxPacketSizeV>
void BulkTX<Derived, MessageTypeT, MaxPacketSizeV>::write(
    const MessageType *msgs, uint32_t num_msgs) {
    const uint32_t *end = msgs + num_msgs;
    while (msgs != end) msgs += write_impl(msgs, end - msgs);
}

template <class Derived, class MessageTypeT, uint16_t MaxPacketSizeV>
uint32_t BulkTX<Derived, MessageTypeT, MaxPacketSizeV>::write_nonblock(
    const MessageType *msgs, uint32_t num_msgs) {
    uint32_t total_sent = 0, sent = 1;
    while (total_sent < num_msgs && sent != 0) {
        sent = write_impl(msgs + total_sent, num_msgs - total_sent, true);
        total_sent += sent;
    }
    return total_sent;
}

template <class Derived, class MessageTypeT, uint16_t MaxPacketSizeV>
void BulkTX<Derived, MessageTypeT, MaxPacketSizeV>::send_now() {
    auto buffer = writing.send_later.exchange(nullptr, mo_acq);
    if (buffer == nullptr)
        // Either the write function or the timeout_handler already cleared
        // the send_later flag.
        return;
    CRTP(Derived).cancel_timeout();

    // Indicate to any handlers interrupting us that we intend to send a buffer.
    writing.send_now.store(buffer, mo_seq); //                               (5)

    // Try to acquire the sending lock.
    wbuffer_t *old = nullptr;
    if (!writing.sending.compare_exchange_strong(old, buffer, mo_seq)) //    (6)
        // If we couldn't get the lock, whoever has the lock will send the
        // data and clear send-now.
        return;

    // ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ acq >>>sending<<<
    // We now own the sending lock

    // It is possible that the tx_callback ran between (5) and (6), so check
    // the send-now flag again.
    auto send_now = writing.send_now.load(mo_seq);
    if (send_now == nullptr) {
        // If the buffer was already sent, release the sending lock and return.
        writing.sending.store(nullptr, mo_seq); // ▲▲▲
        return;
    }

    // Us having the sending lock also means that the timeout and the
    // tx_callback cannot have concurrent access to writing.active_writebuffer
    // Therefore, acquiring it must succeed.
    auto send_buffer = writing.active_writebuffer.exchange(nullptr, mo_seq);
    // ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ acq <<<active_writebuffer>>>
    CS_MIDI_USB_ASSERT(send_buffer == buffer);
    CS_MIDI_USB_ASSERT(send_now == buffer);
    // Now that we own both the buffer and the sending lock, we can send it
    writing.send_now.store(nullptr, mo_rlx);
    // Prepare the other buffer to be filled
    auto next_buffer = other_buf(send_buffer);
    next_buffer->size = 0;
    // ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ rel <<<active_writebuffer>>>
    writing.active_writebuffer.store(next_buffer, mo_rel);
    // Send the current buffer
    CRTP(Derived).tx_start(send_buffer->buffer, send_buffer->size);
    // ------------------------------------------------------------------------- (sending lock is released by the tx_callback)
    return;
}

template <class Derived, class MessageTypeT, uint16_t MaxPacketSizeV>
void BulkTX<Derived, MessageTypeT, MaxPacketSizeV>::reset(
    uint16_t packet_size) {
    writing.packet_size = packet_size;
    writing.buffers[0].size = 0;
    writing.buffers[1].size = 0;
    writing.active_writebuffer.store(&writing.buffers[0], mo_rlx);
    writing.send_later.store(nullptr, mo_rlx);
    writing.send_now.store(nullptr, mo_rlx);
    writing.sending.store(nullptr, mo_rlx);
}

template <class Derived, class MessageTypeT, uint16_t MaxPacketSizeV>
bool BulkTX<Derived, MessageTypeT, MaxPacketSizeV>::is_done() const {
    return writing.sending.load(mo_acq) == nullptr &&
           writing.send_later.load(mo_acq) == nullptr &&
           writing.send_now.load(mo_acq) == nullptr;
}

template <class Derived, class MessageTypeT, uint16_t MaxPacketSizeV>
uint32_t BulkTX<Derived, MessageTypeT, MaxPacketSizeV>::write_impl(
    const MessageType *msgs, uint32_t num_msgs) {
    if (num_msgs == 0)
        return 0;

    // Try to get access to an available buffer
    wbuffer_t *buffer = writing.active_writebuffer.exchange(nullptr, mo_acq);
    // If that failed, return without blocking, caller may retry until we get
    // a buffer we can use
    if (buffer == nullptr)
        return 0;

    // ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ acq <<<active_writebuffer>>>
    // At this point we have a buffer, and we have exclusive access to it,
    // but it may still be full
    // CS_MIDI_USB_ASSERT(writing.send_now.load(mo_rlx) == nullptr);

    auto size = buffer->size;
    CS_MIDI_USB_ASSERT(size != SizeReserved);
    size_t avail_size = writing.packet_size - size;
    auto copy_size_zu = std::min<size_t>(avail_size, num_msgs * sizeof(*msgs));
    auto copy_size = static_cast<uint16_t>(copy_size_zu);
    if (copy_size > 0) {
        std::memcpy(buffer->buffer + size, msgs, copy_size);
        buffer->size = size + copy_size;
    }
    // ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ rel <<<active_writebuffer>>>
    // Release access to the buffer
    writing.active_writebuffer.store(buffer, mo_seq);

    if (copy_size > 0) {
        // If we completely filled this buffer in one go, send it now.
        if (size == 0 && copy_size == writing.packet_size) {
            CS_MIDI_USB_ASSERT(writing.send_later.load(mo_rlx) == nullptr);
            CS_MIDI_USB_ASSERT(writing.send_now.load(mo_rlx) == nullptr);
            writing.send_now.store(buffer, mo_rel); // TODO: can be relaxed
        }
        // If this is the first data in the buffer, schedule it to be sent later.
        else if (size == 0) {
            CS_MIDI_USB_ASSERT(writing.send_later.load(mo_rlx) == nullptr);
            writing.send_later.store(buffer, mo_rel);
            CRTP(Derived).start_timeout();
        }
        // If this buffer was partially filled before and now full, send it now.
        else if (size + copy_size == writing.packet_size) {
            auto send_buffer = writing.send_later.exchange(nullptr, mo_acq);
            if (send_buffer != nullptr) {
                CS_MIDI_USB_ASSERT(writing.send_now.load(mo_rlx) == nullptr);
                CS_MIDI_USB_ASSERT(send_buffer == buffer);
                CRTP(Derived).cancel_timeout();
                writing.send_now.store(buffer, mo_rel); // TODO: can be relaxed
            }
        }
    }

    // An interrupt may have attempted to send the buffer while we owned it.
    if (writing.send_now.load(mo_seq) == nullptr) //                         (1)
        // If that's not the case, we can safely return.
        return copy_size / sizeof(*msgs);

    // Otherwise, it's our job to send the data that the interrupt failed to
    // send.

    // Try to acquire the sending lock.
    wbuffer_t *old = nullptr;
    if (!writing.sending.compare_exchange_strong(old, buffer, mo_seq)) //    (2)
        // If we couldn't get the lock, whoever has the lock will send the
        // data and clear send-now.
        return copy_size / sizeof(*msgs);
    // ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ acq >>>sending<<<
    // We now own the sending lock

    // It is possible that the tx_callback ran between (1) and (2), so check
    // the send-now flag again.
    auto send_now = writing.send_now.load(mo_seq);
    if (send_now == nullptr) {
        // If the buffer was already sent, release the sending lock and return.
        writing.sending.store(nullptr, mo_seq); // ▲▲▲
        return copy_size / sizeof(*msgs);
    }

    // Us having the sending lock also means that the timeout and the
    // tx_callback cannot have concurrent access to writing.active_writebuffer
    // Therefore, acquiring it must succeed.
    auto send_buffer = writing.active_writebuffer.exchange(nullptr, mo_seq);
    // ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ acq <<<active_writebuffer>>>
    CS_MIDI_USB_ASSERT(send_buffer == buffer);
    CS_MIDI_USB_ASSERT(send_now == buffer);
    // Now that we own both the buffer and the sending lock, we can send it
    writing.send_now.store(nullptr, mo_rlx);
    // Prepare the other buffer to be filled
    auto next_buffer = other_buf(send_buffer);
    next_buffer->size = 0;
    // ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ rel <<<active_writebuffer>>>
    writing.active_writebuffer.store(next_buffer, mo_rel);
    // Send the current buffer
    CRTP(Derived).tx_start(send_buffer->buffer, send_buffer->size);
    // ------------------------------------------------------------------------- (sending lock is released by the tx_callback)

    // TODO: if copy_size == 0, we could retry here

    return copy_size / sizeof(*msgs);
}

template <class Derived, class MessageTypeT, uint16_t MaxPacketSizeV>
void BulkTX<Derived, MessageTypeT, MaxPacketSizeV>::timeout_callback() {
    auto buffer = writing.send_later.exchange(nullptr, mo_acq);
    if (buffer == nullptr)
        // Either the write function or the send_now function already cleared
        // the send_later flag.
        return;

    // Indicate to any handlers interrupting us that we intend to send a buffer.
    writing.send_now.store(buffer, mo_seq); //                               (7)

    // Try to acquire the sending lock.
    wbuffer_t *old = nullptr;
    if (!writing.sending.compare_exchange_strong(old, buffer, mo_seq)) //    (8)
        // If we couldn't get the lock, whoever has the lock will send the
        // data and clear send-now.
        return;
    // ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ acq >>>sending<<<
    // We now own the sending lock

    // It is possible that the tx_callback ran between (7) and (8), so check
    // the send-now flag again.
    auto send_now = writing.send_now.load(mo_seq);
    if (send_now == nullptr) {
        // If the buffer was already sent, release the sending lock and return.
        writing.sending.store(nullptr, mo_seq); // ▲▲▲
        return;
    }

    // Us having the sending lock also means that the timeout and the
    // tx_callback cannot have concurrent access to writing.active_writebuffer
    // Therefore, acquiring it must succeed.
    if (auto act = writing.active_writebuffer.exchange(nullptr, mo_seq)) {
        // ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ acq <<<active_writebuffer>>>
        CS_MIDI_USB_ASSERT(act == buffer);
        CS_MIDI_USB_ASSERT(send_now == buffer);
        // Now that we own both the buffer and the sending lock, we can send it
        writing.send_now.store(nullptr, mo_rlx);
        // Prepare the other buffer to be filled
        auto next_buffer = other_buf(act);
        next_buffer->size = 0;
        // ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ rel <<<active_writebuffer>>>
        writing.active_writebuffer.store(next_buffer, mo_rel);
        // Send the current buffer
        CRTP(Derived).tx_start_timeout(buffer->buffer, buffer->size);
        // --------------------------------------------------------------------- (sending lock is released by the tx_callback)
        return;
    }
    // The write function owns the buffer.
    // TODO: only valid if this is an interrupt handler, see tx_callback comment
    // ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ rel >>>sending<<<
    writing.sending.store(nullptr, mo_seq);
    return;
}

template <class Derived, class MessageTypeT, uint16_t MaxPacketSizeV>
void BulkTX<Derived, MessageTypeT, MaxPacketSizeV>::tx_callback() {
    // ------------------------------------------------------------------------- (we still own the sending lock)
    wbuffer_t *sent_buffer = writing.sending.load(mo_acq);
    CS_MIDI_USB_ASSERT(sent_buffer != nullptr);

    // Check if anyone tried to send the next buffer while the previous one
    // was still being sent
    wbuffer_t *send_next = writing.send_now.load(mo_seq);
    if (send_next) {
        CS_MIDI_USB_ASSERT(send_next != sent_buffer);
        // We already own the sending lock.
        if (auto act = writing.active_writebuffer.exchange(nullptr, mo_seq)) {
            // ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ acq <<<active_writebuffer>>>
            CS_MIDI_USB_ASSERT(act == send_next);
            writing.send_now.store(nullptr, mo_rlx);
            sent_buffer->size = 0;
            writing.sending.store(send_next, mo_seq);
            // ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ rel <<<active_writebuffer>>>
            writing.active_writebuffer.store(sent_buffer, mo_seq);
            CRTP(Derived).tx_start_isr(send_next->buffer, send_next->size);
            return;
            // ----------------------------------------------------------------- (sending lock is released by the next tx_callback)
        }
        // Someone else already holds the active_buffer lock.
        // We own the sending lock, but not the buffer to be sent. Since the
        // timeout and send_now can only own the buffer if they also own the
        // sending lock, it must be the write function that owns the buffer.
        // The write function always checks the send-now flag after releasing
        // the buffer, so we can safely release our sending lock and return.
        // In the case of interrupts/signal handlers, our release of the lock
        // happens-before the release of the release of the active buffer in
        // the write function, so there would be no need to check send_now
        // again after releasing. In different threads, we would need to check
        // again, because the write thread could try to acquire the sending
        // lock before we released it.
        // ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ rel >>>sending<<<
        writing.sending.store(nullptr, mo_seq);
        return;

        // TODO: can we do it for the threaded case? At first sight, we should
        //       try to acquire the active write buffer first, and then try
        //       to acquire the sending lock again.
        //       Alternatively, we could add a “main-should-wait” flag to
        //       indicate that the write function should do a busy wait because
        //       we'll release the sending lock soon.
    }

    // Release the “sending” lock
    // ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ rel >>>sending<<<
    writing.sending.store(nullptr, mo_seq);

    // Someone may have tried sending the other buffer while we were still
    // holding the sending lock, so check the send_now flag (again).
    send_next = writing.send_now.load(mo_seq); //                            (3)
    if (send_next == nullptr)
        // No one tried to send before, and if they try to send later, they will
        // be able to get the lock to do so.
        return;

    // Someone tried to send. We must try to send now.
    wbuffer_t *old = nullptr;
    if (!writing.sending.compare_exchange_strong(old, send_next, mo_seq)) // (4)
        // If this fails, someone else must have been able to acquire the
        // sending lock in the meantime, and will be able to make progress.
        return;

    // The send_now flag must still be true: either the timeout fired between
    // (3) and (4) and clears it, in which case it holds on to the sending lock,
    // or it failed to get both the sending lock and the buffer, and in that
    // case it doesn't clear send-now. Similar for the main program.
    CS_MIDI_USB_ASSERT(writing.send_now.load(mo_seq) == send_next);

    // ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ acq >>>sending<<<
    // We now hold the sending lock (again)

    // This is the same as earlier
    if (auto act = writing.active_writebuffer.exchange(nullptr, mo_seq)) {
        // ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ acq <<<active_writebuffer>>>
        CS_MIDI_USB_ASSERT(act == send_next);
        writing.send_now.store(nullptr, mo_rlx);
        sent_buffer->size = 0;
        writing.sending.store(send_next, mo_seq);
        // ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ rel <<<active_writebuffer>>>
        writing.active_writebuffer.store(sent_buffer, mo_seq);
        CRTP(Derived).tx_start_isr(send_next->buffer, send_next->size);
        return;
        // --------------------------------------------------------------------- (sending lock is released by the next tx_callback)
    }
    // ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ rel >>>sending<<<
    writing.sending.store(nullptr, mo_seq);

    // TODO: same as above
}

END_CS_NAMESPACE

#undef CS_MIDI_USB_ASSERT
