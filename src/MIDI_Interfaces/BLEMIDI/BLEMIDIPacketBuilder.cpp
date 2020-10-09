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

    if (!hasSpaceFor(2))
        return false; // Buffer full

    // Normal running status is interrupted by SysEx
    runningHeader = 0;

    const uint8_t timestampLSB = getTimestampLSB(timestamp);

    // Start of SysEx
    buffer[index++] = timestampLSB;
    buffer[index++] = SysExStart;

    // If the entire message fits in this packet (including SysEx start/end
    // and timestamps)
    if (hasSpaceFor(length + 2)) {
        // Copy data
        while (length-- > 0)
            buffer[index++] = *data++;
        ++length;
        // End of SysEx
        buffer[index++] = timestampLSB;
        buffer[index++] = SysExEnd;
        // Message was finished, no continuation
        data = nullptr;
    }
    // Message doesn't fit this packet:
    // Start the message, and write until the packet is full
    else {
        // Copy as much data as possible
        while (length-- > 0 && index < capacity)
            buffer[index++] = *data++;
        ++length;
        // data is not set to nullptr to let the caller know where to start
        // sending the next continuation packet,
    }
    return true;
}

void BLEMIDIPacketBuilder::continueSysEx(const uint8_t *&data, size_t &length,
                                         uint16_t timestamp) {
    if (index != 0) {
        ERROR(F("Error: SysEx should continue in new packet"), 0x2324);
        reset(); // LCOV_EXCL_LINE
    }            // LCOV_EXCL_LINE

    initBuffer(timestamp);

    // Copy as much data as possible
    while (length-- > 0 && index < capacity)
        buffer[index++] = *data++;
    ++length;

    // If the entire message fits in this packet (including SysEx end
    // and timestamps)
    if (length == 0 && hasSpaceFor(2)) {
        // End of SysEx
        buffer[index++] = getTimestampLSB(timestamp);
        buffer[index++] = SysExEnd;
        // Message was finished, no continuation
        data = nullptr;
    } else {
        // data is not set to nullptr to let the caller know where to start
        // sending the next continuation packet,
    }
}

END_CS_NAMESPACE
