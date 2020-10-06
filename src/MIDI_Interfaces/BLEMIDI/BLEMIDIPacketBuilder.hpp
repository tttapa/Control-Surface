#pragma once

#include <MIDI_Parsers/MIDI_MessageTypes.hpp>

BEGIN_CS_NAMESPACE

/// Class for building MIDI over Bluetooth Low Energy packets.
class BLEMIDIPacketBuilder {
  private:
    constexpr static uint8_t MAX_CAPACITY = 20;
    uint8_t capacity = MAX_CAPACITY;
    uint8_t index = 0;
    uint8_t runningHeader = 0;
    uint8_t runningTimestamp = 0;
    uint8_t buffer[MAX_CAPACITY] = {};

    constexpr static uint8_t SysExStart 
        = static_cast<uint8_t>(MIDIMessageType::SYSEX_START);
    constexpr static uint8_t SysExEnd 
        = static_cast<uint8_t>(MIDIMessageType::SYSEX_END);

    /// Check if the buffer has space left.
    constexpr bool hasSpaceFor(size_t bytes) const {
        return index < capacity && bytes <= size_t(capacity - index); 
    }

    /// Timestamp[0]: 0b10hh hhhh
    constexpr static uint8_t getTimestampMSB(uint16_t timestamp) {
        return ((timestamp >> 7) & 0x3F) | 0x80;
    }
    /// Timestamp[1]: 0b1lll llll
    constexpr static uint8_t getTimestampLSB(uint16_t timestamp) {
        return timestamp | 0x80;
    }

    /// If this is the first byte/message in the packet, add the header 
    /// containing the 6 most significant bits of the timestamp
    void initBuffer(uint16_t timestamp) {
        if (index == 0)
            buffer[index++] = getTimestampMSB(timestamp);
    }

    /** 
     * @brief   Try adding a 2-byte or 3-byte MIDI channel message to the packet.
     * 
     * @tparam  ThreeBytes 
     *          Set to `true` for a 3-byte message, `false` for a 2-byte message.
     * 
     * @param   header 
     *          MIDI status byte.
     * @param   data1 
     *          MIDI data byte 1.
     * @param   data2 
     *          MIDI data byte 2 (if `ThreeBytes == true`).
     * @param   timestamp 
     *          13-bit BLE-MIDI timestamp.
     * 
     * @retval  true
     *          Successfully added message to the packet.
     * @retval  false 
     *          Buffer is too full, send the current packet, reset the packet
     *          builder, and try again.
     */    
    template <bool ThreeBytes>
    bool addImpl(uint8_t header, uint8_t data1, uint8_t data2, 
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

  public:
    /// Reset the builder to start a new packet.
    void reset() {
        index = 0;
        runningHeader = 0;
    }

    /// Set the maximum capacity of the buffer. Set this to the MTU of the BLE
    /// link.
    void setCapacity(uint8_t capacity) { 
        if (capacity > MAX_CAPACITY)
            throw std::out_of_range("capacity larger than maximum capacity");
        if (capacity < 5)
            throw std::out_of_range("capacity less than 5 bytes");
        this->capacity = capacity; 
    }

    /// Get the size of the current packet.
    uint8_t getSize() const { return index; }
    /// Get a pointer to the packet data buffer.
    const uint8_t *getBuffer() const { return buffer; }

    /// Return the packet as a vector of bytes (used for testing).
    std::vector<uint8_t> asVector() const {
        return std::vector<uint8_t>(buffer, buffer + index);
    }

    /** 
     * @brief   Try adding a 3-byte MIDI channel message to the packet.
     * 
     * @param   header 
     *          MIDI status byte.
     * @param   data1 
     *          MIDI data byte 1.
     * @param   data2 
     *          MIDI data byte 2.
     * @param   timestamp 
     *          13-bit BLE-MIDI timestamp.
     * 
     * @retval  true
     *          Successfully added message to the packet.
     * @retval  false 
     *          Buffer is too full, send the current packet, reset the packet
     *          builder, and try again.
     */   
    bool add3B(uint8_t header, uint8_t data1, uint8_t data2, 
               uint16_t timestamp) {
        constexpr bool ThreeBytes = true;
        return addImpl<ThreeBytes>(header, data1, data2, timestamp);
    }

    /** 
     * @brief   Try adding a 2-byte MIDI channel message to the packet.
     * 
     * @param   header 
     *          MIDI status byte.
     * @param   data1 
     *          MIDI data byte 1.
     * @param   timestamp 
     *          13-bit BLE-MIDI timestamp.
     * 
     * @retval  true
     *          Successfully added message to the packet.
     * @retval  false 
     *          Buffer is too full, send the current packet, reset the packet
     *          builder, and try again.
     */
    bool add2B(uint8_t header, uint8_t data1, uint16_t timestamp) {
        constexpr bool ThreeBytes = false;
        return addImpl<ThreeBytes>(header, data1, 0, timestamp);
    }

    /** 
     * @brief   Try adding a MIDI real-time message to the packet.
     * 
     * @param   rt 
     *          MIDI real-time byte.
     * @param   timestamp 
     *          13-bit BLE-MIDI timestamp.
     * 
     * @retval  true
     *          Successfully added message to the packet.
     * @retval  false 
     *          Buffer is too full, send the current packet, reset the packet
     *          builder, and try again.
     */  
    bool addRealTime(uint8_t rt, uint16_t timestamp) {
        initBuffer(timestamp);

        if (!hasSpaceFor(2))
            return false; // Buffer full

        buffer[index++] = getTimestampLSB(timestamp);
        buffer[index++] = rt;
        runningTimestamp = 0; // Re-send the timestamp next time

        return true;
    }

    /**
     * @brief   Try adding a SysEx message to the packet.
     * 
     * @param[in]   data 
     *              Pointer to the first data byte of the SysEx message. 
     *              (Not the SysExStart byte!)
     * @param[in]   length 
     *              The number of data bytes in the SysEx message. 
     *              (Not including the SysExStart and SysExEnd bytes!)
     * @param[out]  cont 
     *              Pointer to the first data byte to send in the next packet.
     *              `nullptr` if message was finished.
     * @param[in]   timestamp 
     *              13-bit BLE-MIDI timestamp.
     * 
     * @return  The remaining number of data bytes to send in the next packet.
     * 
     * If the message fits in a single packet, `0` is returned (no remaining
     * data bytes) and `cont == nullptr`.
     * 
     * For example:
     * ~~~cpp
     * const uint8_t *data = (...);
     * size_t length = (...);
     * 
     * BLEMIDIPacketBuilder packetbuilder;
     * const uint8_t *cont; // data continuation
     * length = packetbuilder.addSysEx(data, length, cont, timestamp);
     * while (cont != nullptr) {
     *     sendnow(packetbuilder.getBuffer(), packetbuilder.getSize());
     *     packetbuilder.reset();
     *     length = packetbuilder.continueSysEx(cont, length, cont, timestamp);
     * }
     * ~~~
     */
    size_t addSysEx(const uint8_t *data, size_t length, const uint8_t *&cont,
                    uint16_t timestamp) {
        initBuffer(timestamp);

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
            cont = nullptr;
        }
        // Message doesn't fit this packet:
        // Start the message, and write until the packet is full
        else {
            // Copy as much data as possible
            while (length-- > 0 && index < capacity)
                buffer[index++] = *data++;
            ++length;
            // Tell caller where to start sending the next continuation packet
            cont = data;
        }
        // Return remaining number of data bytes to send
        return length;
    }

    /**
     * @brief   Add a SysEx continuation to the packet.
     * 
     * @param[in]   data 
     *              Pointer to the first data byte of the SysEx message to send
     *              in this continuation packet.
     * @param[in]   length 
     *              The number of remaining data bytes in the SysEx message. 
     *              (Not including the SysExEnd byte!)
     * @param[out]  cont 
     *              Pointer to the first data byte to send in the next packet.
     *              `nullptr` if message was finished.
     * @param[in]   timestamp 
     *              13-bit BLE-MIDI timestamp.
     * 
     * @return  The remaining number of data bytes to send in the next packet.
     * 
     * If the message can be completed in a single packet, `0` is returned (no
     * remaining data bytes) and `cont == nullptr`.
     * 
     * @see @ref addSysEx()
     */
    size_t continueSysEx(const uint8_t *data, size_t length, 
                         const uint8_t *&cont, uint16_t timestamp) {
        if (index != 0) {
            ERROR(F("Error: SysEx should continue in new packet"), 0x2324);
            reset();
        }

        initBuffer(timestamp);

        while (length-- > 0 && index < capacity)
            buffer[index++] = *data++;
        ++length;
        if (length == 0 && hasSpaceFor(2)) {
            buffer[index++] = getTimestampLSB(timestamp);
            buffer[index++] = SysExEnd;
            cont = nullptr;
        } else {
            cont = data;
        }

        return length;
    }
};

END_CS_NAMESPACE
