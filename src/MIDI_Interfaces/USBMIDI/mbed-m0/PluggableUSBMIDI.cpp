#if defined(ARDUINO_ARCH_MBED_RP2040)

#include "PluggableUSBMIDI.hpp"
#include <AH/Debug/Debug.hpp>

#include <cstring> // memcpy

BEGIN_CS_NAMESPACE

#ifdef FATAL_ERRORS
#define CS_MIDI_USB_ASSERT(a) MBED_ASSERT((a))
#else
#define CS_MIDI_USB_ASSERT(a) (void)(a)
#endif

using std::tie;

// ------------------------- CONSTRUCTOR/DESTRUCTOR ------------------------- //

PluggableUSBMIDI::PluggableUSBMIDI() : PluggableUSBModule(2) {
    PluggableUSBD().plug(this);
}

PluggableUSBMIDI::~PluggableUSBMIDI() { PluggableUSBD().deinit(); }

// ----------------------------- INITIALIZATION ----------------------------- //

void PluggableUSBMIDI::init(EndpointResolver &resolver) {
    bulk_in_ep = resolver.endpoint_in(USB_EP_TYPE_BULK, PacketSize);
    bulk_out_ep = resolver.endpoint_out(USB_EP_TYPE_BULK, PacketSize);
    CS_MIDI_USB_ASSERT(resolver.valid());
}

bool PluggableUSBMIDI::connected() const {
    return usb_connected.load(std::memory_order_relaxed);
}

// --------------------------------- USB API -------------------------------- //

void PluggableUSBMIDI::callback_state_change(DeviceState new_state) {
    assert_locked();
    usb_connected.store(new_state == USBDevice::Configured,
                        std::memory_order_relaxed);
}

uint32_t PluggableUSBMIDI::callback_request(const setup_packet_t *setup,
                                            USBDevice::RequestResult *result,
                                            uint8_t **data) {
    assert_locked();
    *result = USBDevice::PassThrough;
    *data = nullptr;
    return 0;
}

bool PluggableUSBMIDI::callback_request_xfer_done(const setup_packet_t *setup,
                                                  bool aborted) {
    assert_locked();
    return false;
}

bool PluggableUSBMIDI::callback_set_configuration(uint8_t configuration) {
    assert_locked();

    PluggableUSBD().endpoint_add(
        bulk_in_ep, PacketSize, USB_EP_TYPE_BULK,
        mbed::callback(this, &PluggableUSBMIDI::in_callback));
    PluggableUSBD().endpoint_add(
        bulk_out_ep, PacketSize, USB_EP_TYPE_BULK,
        mbed::callback(this, &PluggableUSBMIDI::out_callback));

    writing.send_timeout = nullptr;
    writing.timeout.detach();
    writing.buffers[0].size = 0;
    writing.buffers[0].ready_to_send = false;
    writing.buffers[1].size = 0;
    writing.buffers[1].ready_to_send = false;
    writing.active_writebuffer = 0;
    writing.sending = nullptr;

    reading.available = 0;
    reading.read_idx = 0;
    reading.write_idx = 0;
    reading.reading = true;
    read_start(bulk_out_ep, reading.buffers[0].buffer, PacketSize);

    return true;
}

void PluggableUSBMIDI::callback_set_interface(uint16_t interface,
                                              uint8_t alternate) {
    assert_locked();
}

// ---------------------------------- WRITING ----------------------------------

void PluggableUSBMIDI::write(uint32_t msg) {
    write_impl(&msg, 1, false); // blocking
}

void PluggableUSBMIDI::write(const uint32_t *msgs, uint32_t num_msgs) {
    const uint32_t *end = msgs + num_msgs;
    while (msgs != end)
        msgs += write_impl(msgs, end - msgs, false); // blocking
}

uint32_t PluggableUSBMIDI::write_nonblock(const uint32_t *msgs,
                                          uint32_t num_msgs) {
    uint32_t total_sent = 0, sent = 1;
    while (total_sent < num_msgs && sent != 0) {
        sent = write_impl(msgs + total_sent, num_msgs - total_sent, true);
        total_sent += sent;
    }
    return total_sent;
}

void PluggableUSBMIDI::send_now() {
    this->lock();
    uint32_t active_idx, size;
    wbuffer_t *writebuf;
    tie(active_idx, writebuf, size) = read_writebuf_size();
    if (size > 0)
        send_now_impl_nonblock(active_idx);
    else
        this->unlock();
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
 *   (`writebuffer = 0`, which means that the first buffer is the one being 
 *   filled with the outgoing data).
 * - The `write()` function is called.
 * - The message is added to the active buffer and the size is incremented. 
 *   (Writing the data happens inside of a critical section.)
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

std::tuple<uint32_t, PluggableUSBMIDI::wbuffer_t *, uint32_t>
PluggableUSBMIDI::read_writebuf_size() {
    this->assert_locked();
    uint32_t active_idx = writing.active_writebuffer;
    wbuffer_t *writebuffer = &writing.buffers[active_idx];
    uint32_t size = writebuffer->size;
    CS_MIDI_USB_ASSERT(size != SizeReserved);
    return std::make_tuple(active_idx, writebuffer, size);
}

uint32_t PluggableUSBMIDI::write_impl(const uint32_t *msgs, uint32_t num_msgs,
                                      bool nonblocking) {
    // Index of the buffer currently being written to.
    uint32_t active_idx;
    // Pointer to buffer currently being written to.
    wbuffer_t *writebuf;
    // Current size of that buffer (i.e. the first index we can write to).
    uint32_t size;

    this->lock();
    tie(active_idx, writebuf, size) = read_writebuf_size();

    // Make sure that there's space in the buffer for us to write data
    if (size >= PacketSize) {
        // If there's no space
        if (nonblocking) {
            // either return without blocking
            return this->unlock(), 0;
        } else {
            // Or wait until the active writing buffer changes to an empty one
            auto old_idx = active_idx;
            do {
                this->unlock();
                yield(); // spin
                this->lock();
                tie(active_idx, writebuf, size) = read_writebuf_size();
            } while (size >= PacketSize);
            CS_MIDI_USB_ASSERT(old_idx != active_idx);
            CS_MIDI_USB_ASSERT(size == 0);
        }
    }
    CS_MIDI_USB_ASSERT(size < PacketSize);

    // Copy the data into the active buffer
    uint32_t free_size = std::min(PacketSize - size, num_msgs * 4u);
    CS_MIDI_USB_ASSERT(free_size % 4 == 0);
    CS_MIDI_USB_ASSERT(size % 4 == 0);
    size = (size / 4) * 4;
    free_size = (free_size / 4) * 4;
    memcpy(&writebuf->buffer[size], msgs, free_size);
    uint32_t newsize = size + free_size;
    writebuf->size = newsize;
    this->unlock();

    // If the buffer is now full, send it immediately (but don't block)
    if (newsize == PacketSize) {
        send_now_impl_nonblock(active_idx);
    }
    // If this is the first data in the buffer, start a timer that will send
    // the buffer after a given timeout
    else if (size == 0) {
        this->lock();
        wbuffer_t *old = std::exchange(writing.send_timeout, writebuf);
        this->unlock();
        CS_MIDI_USB_ASSERT(old == nullptr);
        std::atomic_signal_fence(std::memory_order_release);
        /* ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ [Timeout] ▼ */
        auto cb = mbed::callback(this, &PluggableUSBMIDI::timeout_callback);
        writing.timeout.attach(std::move(cb), writing.timeout_duration);
    }

    // How many messages were added to the transmit buffer
    return free_size / 4u;
}

/**
 * @pre not locked
 * @post not locked
 */
bool PluggableUSBMIDI::send_now_impl_nonblock(uint32_t active_idx) {
    this->lock();
    // Disable the timeout
    auto old_timeout = std::exchange(writing.send_timeout, nullptr);

    wbuffer_t *writebuffer = &writing.buffers[active_idx];

    // Schedule the active buffer to be sent immediately after the previous one
    bool old_ready = std::exchange(writebuffer->ready_to_send, true);
    if (old_ready)
        // If the send flag was already set, there's nothing left for us to do
        return this->unlock(), true;

    // If we were the ones who set the send flag, try to send the packet now

    // Try to acquire the “sending” lock:
    if (writing.sending != nullptr)
        // If we failed to get the lock, the previous transmission is still in
        // progress, so we can't send now, but sending of our buffer is
        // scheduled using the ready_to_send flag, so we can just return.
        return this->unlock(), false;
    writing.sending = writebuffer;

    // Swap the active write buffer
    CS_MIDI_USB_ASSERT(writing.buffers[!active_idx].size == 0);
    writing.active_writebuffer = !active_idx;

    // Get the size of the buffer to send and atomically set it to reserved.
    // This buffer cannot be sent because writing.sending == nullptr, so no 
    // previous transmission was in progress that could have started sending
    // this buffer from an ISR.
    // Similarly, the timeout callback couldn't have started sending it either
    // because we own the writing.sending lock.
    uint32_t size = std::exchange(writebuffer->size, SizeReserved);
    CS_MIDI_USB_ASSERT(size != SizeReserved);
    CS_MIDI_USB_ASSERT(size != 0);

    // Actually send the buffer
    write_start_sync(writebuffer->buffer, size);
    // “sending” lock is released in USB callback

    this->unlock();
    if (old_timeout != nullptr)
        writing.timeout.detach();

    return true;
}

void PluggableUSBMIDI::send_in_callback(uint32_t sendbuf_idx) {
    // Only called in scenarios where we already own “sending” lock

    // Get the size of the buffer to send and atomically set it to reserved
    wbuffer_t *sendbuffer = &writing.buffers[sendbuf_idx];
    uint32_t size = std::exchange(sendbuffer->size, SizeReserved);
    CS_MIDI_USB_ASSERT(size != SizeReserved);
    CS_MIDI_USB_ASSERT(size != 0);

    // Actually send the buffer
    write_start_sync(sendbuffer->buffer, size);
}

void PluggableUSBMIDI::timeout_callback() {
    this->lock(); // Note: we're in an ISR
    std::atomic_signal_fence(std::memory_order_acquire);
    /* ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ [Timeout] ▲ */

    // Check which buffer (if any) to send
    wbuffer_t *sendbuffer = std::exchange(writing.send_timeout, nullptr);
    if (sendbuffer == nullptr)
        return this->unlock();

    // Schedule the active buffer to be sent immediately after the previous one
    bool old_ready = std::exchange(sendbuffer->ready_to_send, true);
    if (old_ready)
        // If the send flag was already set, there's nothing left for us to do
        return this->unlock();

    // Try to get the send lock
    if (writing.sending != nullptr)
        // If we failed to get the lock, he previous transmission is still in
        // progress, so we can't send now, but sending of our buffer is
        // scheduled using the ready_to_send flag, so we can just return.
        return this->unlock();
    writing.sending = sendbuffer;

    // Swap the buffers
    uint32_t sendbuf_idx = sendbuffer - writing.buffers;
    CS_MIDI_USB_ASSERT(writing.buffers[!sendbuf_idx].size == 0);
    writing.active_writebuffer = !sendbuf_idx;

    // Reserve and send the buffer
    send_in_callback(sendbuf_idx);
    return this->unlock();
}

void PluggableUSBMIDI::write_start_sync(uint8_t *buffer, uint32_t size) {
    CS_MIDI_USB_ASSERT(size <= PacketSize);
    // digitalWrite(LED_BUILTIN, HIGH);
    std::atomic_signal_fence(std::memory_order_release);
    /* ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ [Write-Start] ▼ */
    write_start(bulk_in_ep, buffer, size);
}

void PluggableUSBMIDI::in_callback() {
    std::atomic_signal_fence(std::memory_order_acquire);
    /* ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ [Write-Start] ▲ */

    assert_locked();
    // digitalWrite(LED_BUILTIN, LOW);
    write_finish(bulk_in_ep);

    // Release the “sending” lock
    wbuffer_t *sent = std::exchange(writing.sending, nullptr);
    uint32_t sent_idx = sent - writing.buffers;
    uint32_t next_idx = !sent_idx;
    wbuffer_t *next = &writing.buffers[next_idx];

    // Check if the other buffer was scheduled to be sent immediately after the
    // one we just sent
    uint32_t next_ready = next->ready_to_send;
    if (next_ready) {
        // Acquire the “sending” lock again
        wbuffer_t *old_sending = std::exchange(writing.sending, next);
        // Since interrupt handlers are atomic blocks, acquiring the lock cannot
        // fail. If multiple threads are used, an extra flag is needed to
        // prevent the main thread and the timeout thread from stealing the lock
        // after we released it.
        CS_MIDI_USB_ASSERT(old_sending == nullptr);

        // The transmission is done, reset the size of the buffer to 0 so the
        // main thread can write to it again.
        sent->ready_to_send = false;
        uint32_t oldsize = std::exchange(sent->size, 0);
        CS_MIDI_USB_ASSERT(oldsize == SizeReserved);

        // Swap the buffers
        writing.active_writebuffer = !next_idx;

        // Send it
        send_in_callback(next_idx);
    } else {
        // The transmission is done, reset the size of the buffer to 0 so the
        // main thread can write to it again.
        sent->ready_to_send = false;
        uint32_t oldsize = std::exchange(sent->size, 0);
        CS_MIDI_USB_ASSERT(oldsize == SizeReserved);
    }
}

// ---------------------------------- READING ----------------------------------

uint32_t PluggableUSBMIDI::read() {
    this->lock();
    // Check if there are any bytes available for reading
    uint32_t available = reading.available;
    if (available == 0)
        return this->unlock(), 0;

    // Get the buffer with received data
    uint32_t r = reading.read_idx;
    rbuffer_t &readbuffer = reading.buffers[r];

    // Read the data from the buffer (data is at least as new as available)
    uint32_t data;
    memcpy(&data, &readbuffer.buffer[readbuffer.index], 4);

    readbuffer.index += 4;
    // If we've read all messages from this buffer
    if (readbuffer.index == readbuffer.size) {
        // Increment the read index (and wrap around)
        r = (r + 1 == NumRxPackets) ? 0 : r + 1;
        reading.read_idx = r;
        reading.available -= 1;
        // There is now space in the queue
        // Check if the next read is already in progress
        bool inprogress = std::exchange(reading.reading, true);
        if (!inprogress) {
            // If not, start the next read now
            uint32_t w = reading.write_idx;
            read_start(bulk_out_ep, reading.buffers[w].buffer, PacketSize);
        }
    }
    return this->unlock(), data;
}

void PluggableUSBMIDI::out_callback() {
    assert_locked();
    CS_MIDI_USB_ASSERT(reading.reading == true);
    // Check how many bytes were read
    uint32_t num_bytes_read = read_finish(bulk_out_ep);
    CS_MIDI_USB_ASSERT(num_bytes_read % 4 == 0);

    // If no bytes were read, start a new read into the same buffer
    uint32_t w = reading.write_idx;
    if (num_bytes_read == 0) {
        read_start(bulk_out_ep, reading.buffers[w].buffer, PacketSize);
        return;
    }

    // Otherwise, store how many bytes were read
    rbuffer_t &writebuffer = reading.buffers[w];
    writebuffer.index = 0;
    writebuffer.size = num_bytes_read;
    // Increment the write index (and wrap around)
    w = (w + 1 == NumRxPackets) ? 0 : w + 1;
    reading.write_idx = w;

    // Update number of available buffers in the queue
    uint32_t available = (reading.available += 1);
    // If there's still space left in the queue, start the next read
    if (available < NumRxPackets)
        read_start(bulk_out_ep, reading.buffers[w].buffer, PacketSize);
    // Otherwise, release the “reading” lock
    else
        reading.reading = false;
}

constexpr uint32_t PluggableUSBMIDI::PacketSize;
constexpr uint32_t PluggableUSBMIDI::SizeReserved;

END_CS_NAMESPACE

#endif