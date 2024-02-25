#include "BulkRX.hpp"

#include <AH/Arduino-Wrapper.h>
#include <AH/Containers/CRTP.hpp>

#include <cassert>

#ifdef FATAL_ERRORS
#define CS_MIDI_USB_ASSERT(a) assert((a))
#else
#define CS_MIDI_USB_ASSERT(a)
#endif

BEGIN_CS_NAMESPACE

template <class Derived, class MessageTypeT, uint16_t MaxPacketSizeV>
void BulkRX<Derived, MessageTypeT, MaxPacketSizeV>::reset(
    uint16_t packet_size) {
    reading.packet_size = packet_size;
    reading.available.store(0, mo_rlx);
    reading.read_idx = 0;
    reading.write_idx.store(0, mo_rlx);
    reading.reading.store(true, mo_rlx);
    CRTP(Derived).rx_start(reading.buffers[0].buffer, reading.packet_size);
}

template <class Derived, class MessageTypeT, uint16_t MaxPacketSizeV>
bool BulkRX<Derived, MessageTypeT, MaxPacketSizeV>::read(MessageType &message) {
    // Check if there are any bytes available for reading
    uint32_t available = reading.available.load(mo_acq);
    if (available == 0)
        return false;

    // Get the buffer with received data
    uint32_t r = reading.read_idx;
    rbuffer_t &read_buffer = reading.buffers[r];

    // Read the data from the buffer (data is at least as new as available)
    memcpy(&message, &read_buffer.buffer[read_buffer.index],
           sizeof(MessageType));

    static_assert(MaxPacketSize % sizeof(MessageType) == 0, "");
    CS_MIDI_USB_ASSERT(reading.packet_size % sizeof(MessageType) == 0);
    read_buffer.index += sizeof(MessageType);
    // If we've read all messages from this buffer
    if (read_buffer.index == read_buffer.size) {
        // Increment the read index (and wrap around)
        r = (r + 1 == NumRxPackets) ? 0 : r + 1;
        reading.read_idx = r;
        reading.available.fetch_sub(1, mo_rel);
        // There is now space in the queue
        // Check if the next read is already in progress
        if (reading.reading.exchange(true, mo_acq) == false) {
            // If not, start the next read now
            uint32_t w = reading.write_idx.load(mo_rlx);
            CRTP(Derived).rx_start(reading.buffers[w].buffer,
                                   reading.packet_size);
        }
    }

    return true;
}

template <class Derived, class MessageTypeT, uint16_t MaxPacketSizeV>
void BulkRX<Derived, MessageTypeT, MaxPacketSizeV>::rx_callback(
    uint32_t num_bytes_read) {
    CS_MIDI_USB_ASSERT(reading.reading.load(mo_rlx) == true);
    num_bytes_read -= num_bytes_read % sizeof(MessageType);

    // If no bytes were read, start a new read into the same buffer
    uint32_t w = reading.write_idx.load(mo_rlx);
    if (num_bytes_read == 0) {
        CRTP(Derived).rx_start_isr(reading.buffers[w].buffer,
                                   reading.packet_size);
        return;
    }

    // Otherwise, store how many bytes were read
    rbuffer_t &write_buffer = reading.buffers[w];
    write_buffer.index = 0;
    write_buffer.size = num_bytes_read;
    // Increment the write index (and wrap around)
    w = (w + 1 == NumRxPackets) ? 0 : w + 1;
    reading.write_idx.store(w, mo_rlx);

    // Update number of available buffers in the queue
    uint32_t available = reading.available.fetch_add(1, mo_acq_rel) + 1;
    // If there's still space left in the queue, start the next read
    if (available < NumRxPackets)
        CRTP(Derived).rx_start_isr(reading.buffers[w].buffer,
                                   reading.packet_size);
    // Otherwise, release the “reading” lock
    else
        reading.reading.store(false, mo_rel);
}

END_CS_NAMESPACE

#undef CS_MIDI_USB_ASSERT
