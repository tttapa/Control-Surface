#include "BLEMIDIPacketBuilder.hpp"

BEGIN_CS_NAMESPACE

template <bool ThreeBytes>
bool BLEMIDIPacketBuilder::addImpl(uint8_t header, uint8_t data1, uint8_t data2,
                                   uint16_t timestamp) {
    initBuffer(timestamp);

    uint8_t timestampLSB = getTimestampLSB(timestamp);

    // If the header is the same as the previous message, use running status
    if (header == runningHeader) {
        // If the timestamp is the same, no need to send it again
        if (timestampLSB == runningTimestamp) {
            if (!hasSpaceFor(1 + ThreeBytes))
                return false; // Buffer full
            buffer[index++] = data1;
            if (ThreeBytes)
                buffer[index++] = data2;
        }
        // Timestamp is different, send again
        else {
            if (!hasSpaceFor(2 + ThreeBytes))
                return false; // Buffer full
            runningTimestamp = timestampLSB;
            buffer[index++] = timestampLSB;
            buffer[index++] = data1;
            if (ThreeBytes)
                buffer[index++] = data2;
        }
    }
    // If the header is different, running status is not possible, send all
    else {
        if (!hasSpaceFor(3 + ThreeBytes))
            return false; // Buffer full
        runningHeader = header;
        runningTimestamp = timestampLSB;
        buffer[index++] = timestampLSB;
        buffer[index++] = header;
        buffer[index++] = data1;
        if (ThreeBytes)
            buffer[index++] = data2;
    }
    return true;
}

void BLEMIDIPacketBuilder::reset() {
    index = 0;
    runningHeader = 0;
}

void BLEMIDIPacketBuilder::setCapacity(uint8_t capacity) {
    if (capacity > MAX_CAPACITY)
        ERROR(F("capacity larger than maximum capacity"), 0x2020);
    if (capacity < 5)
        ERROR(F("capacity less than 5 bytes"), 0x2005);
    this->capacity = capacity;
}

bool BLEMIDIPacketBuilder::add3B(uint8_t header, uint8_t data1, uint8_t data2,
                                 uint16_t timestamp) {
    constexpr bool ThreeBytes = true;
    return addImpl<ThreeBytes>(header, data1, data2, timestamp);
}

bool BLEMIDIPacketBuilder::add2B(uint8_t header, uint8_t data1,
                                 uint16_t timestamp) {
    constexpr bool ThreeBytes = false;
    return addImpl<ThreeBytes>(header, data1, 0, timestamp);
}

bool BLEMIDIPacketBuilder::addRealTime(uint8_t rt, uint16_t timestamp) {
    initBuffer(timestamp);

    if (!hasSpaceFor(2))
        return false; // Buffer full

    buffer[index++] = getTimestampLSB(timestamp);
    buffer[index++] = rt;
    runningTimestamp = 0; // Re-send the timestamp next time

    return true;
}

bool BLEMIDIPacketBuilder::addSysEx(const uint8_t *&data, size_t &length,
                                    uint16_t timestamp) {
    initBuffer(timestamp);

    // We can't do anything with an empty message
    if (length == 0)
        return true;

    // If the first byte is a SysExStart byte we first have to write a
    // timestamp + SysExStart.
    if (*data == SysExStart) {
        // We need space for at least the timestamp and a SysExStart
        if (!hasSpaceFor(2))
            return false; // Buffer full

        // Normal running status is interrupted by SysEx
        runningHeader = 0;

        const uint8_t timestampLSB = getTimestampLSB(timestamp);

        // Start of SysEx
        buffer[index++] = timestampLSB;
        buffer[index++] = SysExStart;
        ++data; // First byte was added
        --length;
    }

    // Copy the rest of the data, and terminate the message if necessary
    continueSysEx(data, length, timestamp);
    return true;
}

void BLEMIDIPacketBuilder::continueSysEx(const uint8_t *&data, size_t &length,
                                         uint16_t timestamp) {
    initBuffer(timestamp);

    // Copy as much data as possible, but stop before the last byte, which
    // could be a SysExEnd (and should be handled differently than data bytes)
    while (length-- > 1 && index < capacity)
        buffer[index++] = *data++;

    // If everything fit into the buffer
    if (length == 0) {
        // End of SysEx
        if (*data == SysExEnd) {
            if (hasSpaceFor(2)) {
                buffer[index++] = getTimestampLSB(timestamp);
                buffer[index++] = SysExEnd;
                // Message was finished, no continuation
                data = nullptr;
            } else {
                // Send the SysExEnd byte next time
                ++length;
            }
        }
        // End of chunk but not end of SysEx
        else {
            if (hasSpaceFor(1)) {
                buffer[index++] = *data;
                // Message was finished, no continuation
                data = nullptr;
            } else {
                // Send the last byte next time
                ++length;
            }
        }
    }
    // If the while loop stopped because the buffer was full
    else {
        // data is not set to nullptr to let the caller know where to start
        // sending the next continuation packet,
        ++length;
    }
}

END_CS_NAMESPACE
