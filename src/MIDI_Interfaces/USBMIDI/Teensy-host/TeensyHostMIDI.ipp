#define CS_MIDI_USB_ASSERT(a)                                                  \
    do {                                                                       \
        if (!(a)) {                                                            \
            Serial.println("USB Host MIDI: Assertion failed: " #a);            \
            Serial.flush();                                                    \
            while (1)                                                          \
                yield();                                                       \
        }                                                                      \
    } while (0)

BEGIN_CS_NAMESPACE

constexpr static auto mo_rel = std::memory_order_release;
constexpr static auto mo_acq = std::memory_order_acquire;
constexpr static auto mo_rlx = std::memory_order_relaxed;
constexpr static auto mo_acq_rel = std::memory_order_acq_rel;

// ------------------------------- INITIALIZATION ------------------------------

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::init() {
    contribute_Pipes(mypipes, len(mypipes));
    contribute_Transfers(mytransfers, len(mytransfers));
    contribute_String_Buffers(mystring_bufs, len(mystring_bufs));
    rxpipe = nullptr;
    txpipe = nullptr;
    driver_ready_for_device(this);
}

template <uint16_t MaxPacketSize>
bool TeensyHostMIDI<MaxPacketSize>::claim(Device_t *dev, int type,
                                          const uint8_t *descriptors,
                                          uint32_t len) {
    // only claim at interface level
    if (type != 1)
        return false;

    if (!claim_if_midi(dev, type, descriptors, len)) {
        USBHost::println_("This interface is not MIDI");
        return false;
    }
    USBHost::println_("claimed");

    // if an IN endpoint was found, create its pipe
    if (rx_ep && rx_size <= PacketSize) {
        rxpipe = new_Pipe(dev, rx_ep_type, rx_ep, 1, rx_size);
        if (rxpipe) {
            rxpipe->callback_function = rx_callback;

            reading.available.store(0, mo_rlx);
            reading.read_idx.store(0, mo_rlx);
            reading.write_idx.store(0, mo_rlx);
            reading.reading.store(true, mo_rlx);
            read_start_isr(reading.buffers[0].buffer, PacketSize);
        }
    } else {
        rxpipe = nullptr;
    }

    // if an OUT endpoint was found, create its pipe
    if (tx_ep && tx_size <= PacketSize) {
        txpipe = new_Pipe(dev, tx_ep_type, tx_ep, 0, tx_size);
        if (txpipe) {
            txpipe->callback_function = tx_callback;

            writing.send_timeout.store(nullptr, mo_rlx);
            writing.buffers[0].size.store(0, mo_rlx);
            writing.buffers[0].ready_to_send.store(false, mo_rlx);
            writing.buffers[1].size.store(0, mo_rlx);
            writing.buffers[1].ready_to_send.store(false, mo_rlx);
            writing.active_writebuffer.store(0, mo_rlx);
            writing.sending.store(nullptr, mo_rlx);
        }
    } else {
        txpipe = nullptr;
    }

    // claim if either pipe created
    return (rxpipe || txpipe);
}

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::disconnect() {
    rxpipe = nullptr;
    txpipe = nullptr;
}

// ------------------------------ USB CALLBACKS ----------------------------- //

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::rx_callback(const Transfer_t *transfer) {
    if (transfer->driver)
        reinterpret_cast<TeensyHostMIDI *>(transfer->driver)
            ->out_callback(transfer);
}
template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::tx_callback(const Transfer_t *transfer) {
    if (transfer->driver)
        reinterpret_cast<TeensyHostMIDI *>(transfer->driver)
            ->in_callback(transfer);
}

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::timer_event(USBDriverTimer *whichtimer) {
    if (whichtimer == &write_timeout)
        timeout_callback();
}

// ----------------------------- USB READ/WRITE ----------------------------- //

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::write_start(uint8_t *buffer,
                                                uint32_t size) {
    // digitalWrite(3, HIGH);
    __disable_irq();
    if (txpipe)
        queue_Data_Transfer(txpipe, buffer, size, this);
    __enable_irq();
}
template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::write_start_isr(uint8_t *buffer,
                                                    uint32_t size) {
    // digitalWrite(3, HIGH);
    if (txpipe)
        queue_Data_Transfer(txpipe, buffer, size, this);
}
template <uint16_t MaxPacketSize>
uint32_t
TeensyHostMIDI<MaxPacketSize>::write_finish(const Transfer_t *transfer) {
    // digitalWrite(3, LOW);
    // delayMicroseconds(200);
    return transfer->length - ((transfer->qtd.token >> 16) & 0x7FFF);
}

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::read_start(uint8_t *buffer, uint32_t size) {
    __disable_irq();
    if (rxpipe)
        queue_Data_Transfer(rxpipe, buffer, size, this);
    __enable_irq();
}
template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::read_start_isr(uint8_t *buffer,
                                                   uint32_t size) {
    if (rxpipe)
        queue_Data_Transfer(rxpipe, buffer, size, this);
}
template <uint16_t MaxPacketSize>
uint32_t
TeensyHostMIDI<MaxPacketSize>::read_finish(const Transfer_t *transfer) {
    return transfer->length - ((transfer->qtd.token >> 16) & 0x7FFF);
}

// ---------------------------------- WRITING ----------------------------------

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::write(uint32_t msg) {
    write_impl(&msg, 1, false); // blocking
}

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::write(const uint32_t *msgs,
                                          uint32_t num_msgs) {
    const uint32_t *end = msgs + num_msgs;
    while (msgs != end)
        msgs += write_impl(msgs, end - msgs, false); // blocking
}

template <uint16_t MaxPacketSize>
uint32_t TeensyHostMIDI<MaxPacketSize>::write_nonblock(const uint32_t *msgs,
                                                       uint32_t num_msgs) {
    uint32_t total_sent = 0, sent = 1;
    while (total_sent < num_msgs && sent != 0) {
        sent = write_impl(msgs + total_sent, num_msgs - total_sent, true);
        total_sent += sent;
    }
    return total_sent;
}

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::send_now() {
    uint32_t active_idx, size;
    wbuffer_t *writebuf;
    std::tie(active_idx, writebuf, size) = read_writebuf_size();
    if (size > 0)
        send_now_impl_nonblock(active_idx);
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

template <uint16_t MaxPacketSize>
typename TeensyHostMIDI<MaxPacketSize>::writebuf_size_tup
TeensyHostMIDI<MaxPacketSize>::read_writebuf_size() {
    uint32_t active_idx;
    wbuffer_t *writebuffer;
    uint32_t size;
    // active_idx must be newer than active size.
    active_idx = writing.active_writebuffer.load(mo_rlx);
    writebuffer = &writing.buffers[active_idx];
    size = writebuffer->size.load(mo_acq);
    // If the size was set to reserved, the active buffer must have been
    // swapped as well. If both buffers are empty, the active buffer might have
    // been swapped as well (but not necessarily).
    if (size == SizeReserved || size == 0) {
        uint32_t old_idx = active_idx;
        active_idx = writing.active_writebuffer.load(mo_rlx);
        if (old_idx != active_idx) {
            writebuffer = &writing.buffers[active_idx];
            size = writebuffer->size.load(mo_acq);
        }
    }
    CS_MIDI_USB_ASSERT(size != SizeReserved);
    return std::make_tuple(active_idx, writebuffer, size);
}

template <uint16_t MaxPacketSize>
uint32_t TeensyHostMIDI<MaxPacketSize>::write_impl(const uint32_t *msgs,
                                                   uint32_t num_msgs,
                                                   bool nonblocking) {
    if (!txpipe)
        return 0;

    uint32_t active_idx; // Index of the buffer currently being written to.
    wbuffer_t *writebuf; // Pointer to buffer currently being written to.
    uint32_t size;       // Current size of that buffer (i.e. the first index we
                         // can write to).

    // Read the active write buffer and its current size
    std::tie(active_idx, writebuf, size) = read_writebuf_size();
    // Make sure that there's space in the buffer for us to write data
    if (size >= PacketSize) {
        // If there's no space
        if (nonblocking) {
            // either return without blocking
            return 0;
        } else {
            // Or wait until the active writing buffer changes to an empty one
            auto old_idx = active_idx;
            do {
                yield(); // spin
                std::tie(active_idx, writebuf, size) = read_writebuf_size();
            } while (size >= PacketSize);
            CS_MIDI_USB_ASSERT(old_idx != active_idx);
            CS_MIDI_USB_ASSERT(size == 0);
        }
    }
    CS_MIDI_USB_ASSERT(size < PacketSize);

    // Copy the data into the active buffer
    uint32_t free_size = std::min(PacketSize - size, num_msgs * 4u);
    CS_MIDI_USB_ASSERT(free_size % 4 == 0);
    memcpy(&writebuf->buffer[size], msgs, free_size);
    uint32_t newsize = size + free_size;

    // Update the size of the buffer
    while (!writebuf->size.compare_exchange_weak(size, newsize, mo_rel)) {
        // If the size changed while we were writing the data, this means
        // that a sender in a different thread or ISR was/is sending the active
        // buffer (so it is no longer the active buffer).

        // Read the new write buffer and its size.
        std::tie(active_idx, writebuf, size) = read_writebuf_size();
        // CS_MIDI_USB_ASSERT(size == 0); // Only true with cmp_xchg_strong

        // Copy the data into the new buffer
        free_size = std::min(PacketSize - size, num_msgs * 4u);
        memcpy(&writebuf->buffer[size], msgs, free_size);
        newsize = size + free_size;
    }

    // If the buffer is now full, send it immediately (but don't block)
    if (newsize == PacketSize) {
        send_now_impl_nonblock(active_idx);
    }
    // If this is the first data in the buffer, start a timer that will send
    // the buffer after a given timeout
    else if (size == 0) {
        wbuffer_t *old = writing.send_timeout.exchange(writebuf, mo_rel);
        CS_MIDI_USB_ASSERT(old == nullptr);
        std::atomic_signal_fence(mo_rel);
        /* ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ [Timeout] ▼ */
        write_timeout.start(writing.timeout_duration.count());
    }

    // How many messages were added to the transmit buffer
    return free_size / 4u;
}

template <uint16_t MaxPacketSize>
bool TeensyHostMIDI<MaxPacketSize>::send_now_impl_nonblock(
    uint32_t active_idx) {
    // Disable the timeout
    auto old_timeout = writing.send_timeout.exchange(nullptr, mo_acq);
    if (old_timeout != nullptr)
        write_timeout.stop();

    wbuffer_t *writebuffer = &writing.buffers[active_idx];

    // Schedule the active buffer to be sent immediately after the previous one
    bool old_ready = writebuffer->ready_to_send.exchange(true, mo_rlx);
    if (old_ready)
        // If the send flag was already set, there's nothing left for us to do
        return true;

    // If we were the ones who set the send flag, try to send the packet now

    // Try to acquire the “sending” lock:
    wbuffer_t *send = nullptr;
    if (!writing.sending.compare_exchange_strong(send, writebuffer, mo_acq))
        // If we failed to get the lock, he previous transmission is still in
        // progress, so we can't send now, but sending of our buffer is
        // scheduled using the send flag, so we can just return.
        return false;

    // Swap the active write buffer
    CS_MIDI_USB_ASSERT(writing.buffers[!active_idx].size.load(mo_rlx) == 0);
    writing.active_writebuffer.store(!active_idx, mo_rlx);

    // Get the size of the buffer to send and atomically set it to reserved
    uint32_t size = writebuffer->size.exchange(SizeReserved, mo_acq_rel);
    CS_MIDI_USB_ASSERT(size != SizeReserved);
    CS_MIDI_USB_ASSERT(size != 0);

    // Actually send the buffer
    std::atomic_signal_fence(mo_rel);
    /* ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ [Write-Start] ▼ */
    write_start(writebuffer->buffer, size);
    // “sending” lock is released in USB callback
    return true;
}

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::timeout_callback() {
    std::atomic_signal_fence(mo_acq);
    /* ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ [Timeout] ▲ */

    // Check which buffer (if any) to send
    wbuffer_t *sendbuffer = writing.send_timeout.exchange(nullptr, mo_acq);
    if (!sendbuffer)
        return;

    // Schedule the active buffer to be sent immediately after the previous one
    bool old_ready = sendbuffer->ready_to_send.exchange(true, mo_rlx);
    if (old_ready)
        // If the send flag was already set, there's nothing left for us to do
        return;

    // Try to get the send lock
    wbuffer_t *expected = nullptr;
    if (!writing.sending.compare_exchange_strong(expected, sendbuffer, mo_acq))
        // If we failed to get the lock, he previous transmission is still in
        // progress, so we can't send now, but sending of our buffer is
        // scheduled using the send flag, so we can just return.
        return;

    // Swap the buffers
    uint32_t sendbuf_idx = sendbuffer - writing.buffers;
    CS_MIDI_USB_ASSERT(writing.buffers[!sendbuf_idx].size.load(mo_rlx) == 0);
    writing.active_writebuffer.store(!sendbuf_idx, mo_rlx);

    // Get the size of the buffer to send and atomically set it to reserved
    uint32_t size = sendbuffer->size.exchange(SizeReserved, mo_acq_rel);
    CS_MIDI_USB_ASSERT(size != SizeReserved);
    CS_MIDI_USB_ASSERT(size != 0);

    // Actually send the buffer
    std::atomic_signal_fence(mo_rel);
    /* ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ [Write-Start] ▼ */
    write_start_isr(sendbuffer->buffer, size);
}

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::in_callback(const Transfer_t *transfer) {
    std::atomic_signal_fence(mo_acq);
    /* ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲ [Write-Start] ▲ */

    write_finish(transfer);

    // Release the “sending” lock
    wbuffer_t *sent = writing.sending.exchange(nullptr, mo_rel);
    uint32_t sent_idx = sent - writing.buffers;
    uint32_t next_idx = !sent_idx;
    wbuffer_t *next = &writing.buffers[next_idx];

    // Check if the other buffer was scheduled to be sent immediately after the
    // one we just sent
    uint32_t next_ready = next->ready_to_send.load(mo_rlx);
    if (next_ready) {
        // Acquire the “sending” lock again
        wbuffer_t *old_sending = writing.sending.exchange(next, mo_rlx);
        // Since interrupt handlers are atomic blocks, acquiring the lock cannot
        // fail. If multiple threads are used, an extra flag is needed to
        // prevent the main thread and the timeout thread from stealing the lock
        // after we released it.
        CS_MIDI_USB_ASSERT(old_sending == nullptr);

        // The transmission is done, reset the size of the buffer to 0 so the
        // main thread can write to it again.
        sent->ready_to_send.store(false, mo_rlx);
        uint32_t oldsize = sent->size.exchange(0, mo_rel);
        CS_MIDI_USB_ASSERT(oldsize == SizeReserved);

        // Swap the buffers
        writing.active_writebuffer.store(!next_idx, mo_rlx);

        // Get the size of the buffer to send and atomically set it to reserved
        uint32_t size = next->size.exchange(SizeReserved, mo_acq_rel);
        CS_MIDI_USB_ASSERT(size <= PacketSize);
        CS_MIDI_USB_ASSERT(size != 0);

        // Actually send the buffer
        std::atomic_signal_fence(mo_rel);
        /* ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼ [Write-Start] ▼ */
        write_start_isr(next->buffer, size);
    } else {
        // The transmission is done, reset the size of the buffer to 0 so the
        // main thread can write to it again.
        sent->ready_to_send.store(false, mo_rlx);
        uint32_t oldsize = sent->size.exchange(0, mo_rel);
        CS_MIDI_USB_ASSERT(oldsize == SizeReserved);
    }
}

// ---------------------------------- READING ----------------------------------

template <uint16_t MaxPacketSize>
uint32_t TeensyHostMIDI<MaxPacketSize>::read() {
    // Check if there are any bytes available for reading
    uint32_t available = reading.available.load(mo_acq);
    if (available == 0)
        return 0;

    // Get the buffer with received data
    uint32_t r = reading.read_idx.load(mo_rlx);
    rbuffer_t &readbuffer = reading.buffers[r];

    // Read the data from the buffer (data is at least as new as available)
    uint32_t data;
    memcpy(&data, &readbuffer.buffer[readbuffer.index], 4);

    readbuffer.index += 4;
    // If we've read all messages from this buffer
    if (readbuffer.index == readbuffer.size) {
        // Increment the read index (and wrap around)
        r = (r + 1 == NumRxPackets) ? 0 : r + 1;
        reading.read_idx.store(r, mo_rlx);
        reading.available.fetch_sub(1, mo_rel);
        // There is now space in the queue
        // Check if the next read is already in progress
        if (reading.reading.exchange(true, mo_acq) == false) {
            // If not, start the next read now
            uint32_t w = reading.write_idx.load(mo_rlx);
            read_start(reading.buffers[w].buffer, PacketSize);
        }
    }

    return data;
}

template <uint16_t MaxPacketSize>
void TeensyHostMIDI<MaxPacketSize>::out_callback(const Transfer_t *transfer) {
    CS_MIDI_USB_ASSERT(reading.reading.load(mo_rlx) == true);
    // Check how many bytes were read
    uint32_t num_bytes_read = read_finish(transfer);
    CS_MIDI_USB_ASSERT(num_bytes_read % 4 == 0);

    // If no bytes were read, start a new read into the same buffer
    uint32_t w = reading.write_idx.load(mo_rlx);
    if (num_bytes_read == 0) {
        read_start_isr(reading.buffers[w].buffer, PacketSize);
        return;
    }

    // Otherwise, store how many bytes were read
    rbuffer_t &writebuffer = reading.buffers[w];
    writebuffer.index = 0;
    writebuffer.size = num_bytes_read;
    // Increment the write index (and wrap around)
    w = (w + 1 == NumRxPackets) ? 0 : w + 1;
    reading.write_idx.store(w, mo_rlx);

    // Update number of available buffers in the queue
    uint32_t available = reading.available.fetch_add(1, mo_acq_rel) + 1;
    // If there's still space left in the queue, start the next read
    if (available < NumRxPackets)
        read_start_isr(reading.buffers[w].buffer, PacketSize);
    // Otherwise, release the “reading” lock
    else
        reading.reading.store(false, mo_rel);
}

template <uint16_t MaxPacketSize>
constexpr uint32_t TeensyHostMIDI<MaxPacketSize>::PacketSize;
template <uint16_t MaxPacketSize>
constexpr uint32_t TeensyHostMIDI<MaxPacketSize>::SizeReserved;

END_CS_NAMESPACE

#include "TeensyHostMIDI-claim.ipp"
