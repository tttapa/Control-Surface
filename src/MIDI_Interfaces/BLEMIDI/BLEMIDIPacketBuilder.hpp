#pragma once

#include <MIDI_Parsers/MIDI_MessageTypes.hpp>

#include <AH/STL/vector>

BEGIN_CS_NAMESPACE

/// Class for building MIDI over Bluetooth Low Energy packets.
class BLEMIDIPacketBuilder {
  private:
    uint8_t runningHeader = 0;
    uint8_t runningTimestamp = 0;
    std::vector<uint8_t> buffer = std::vector<uint8_t>(0);

    constexpr static const uint8_t SysExStart =
        static_cast<uint8_t>(MIDIMessageType::SysExStart);
    constexpr static const uint8_t SysExEnd =
        static_cast<uint8_t>(MIDIMessageType::SysExEnd);

    /// Check if the buffer has space left.
    bool hasSpaceFor(size_t bytes) const {
        return bytes <= size_t(buffer.capacity() - buffer.size());
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
        if (buffer.empty())
            buffer.push_back(getTimestampMSB(timestamp));
    }

    /** 
     * @brief   Try adding a 2-byte or 3-byte MIDI channel voice message to the
     *          packet.
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
                 uint16_t timestamp);

  public:
    BLEMIDIPacketBuilder(size_t capacity = 20) { buffer.reserve(capacity); }

    /// Reset the builder to start a new packet.
    void reset();

    /// Set the maximum capacity of the buffer. Set this to the MTU of the BLE
    /// link minus three bytes (for notify overhead).
    void setCapacity(uint16_t capacity);

    /// Get the size of the current packet.
    uint16_t getSize() const { return buffer.size(); }
    /// Get a pointer to the packet data buffer.
    const uint8_t *getBuffer() const { return buffer.data(); }
    /// Check if the packet buffer is empty.
    bool empty() const { return buffer.empty(); }

    /// Return the packet as a vector of bytes.
    const std::vector<uint8_t> &getPacket() const { return buffer; }

    /** 
     * @brief   Try adding a 3-byte MIDI channel voice message to the packet.
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
               uint16_t timestamp);

    /** 
     * @brief   Try adding a 2-byte MIDI channel voice message to the packet.
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
    bool add2B(uint8_t header, uint8_t data1, uint16_t timestamp);

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
    bool addRealTime(uint8_t rt, uint16_t timestamp);

    /** 
     * @brief   Try adding a MIDI system common message to the packet.
     * 
     * @param   num_data 
     *          The number of data bytes (0, 1 or 2).
     * @param   header
     *          System common status byte.
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
    bool addSysCommon(uint8_t num_data, uint8_t header, uint8_t data1,
                      uint8_t data2, uint16_t timestamp);

    /**
     * @brief   Try adding (part of) a SysEx message to the packet.
     * 
     * @param[in,out]   data 
     *                  Pointer to the first byte of the SysEx message. 
     *                  At the end, this will point to the first byte to 
     *                  send in the next packet, or `nullptr` if the message was
     *                  finished.
     * @param[in,out]   length 
     *                  The number of bytes in the SysEx message. 
     *                  At the end, this will be set to remaining number of
     *                  bytes to send in the next packet.
     * @param[in]       timestamp 
     *                  13-bit BLE-MIDI timestamp.
     * 
     * @retval  true
     *          Successfully added (part of) the message to the packet.
     * @retval  false 
     *          Buffer is too full, send the current packet, reset the packet
     *          builder, and try again.
     * 
     * If the message fits in a single packet, `length` is set to `0` (no
     * remaining data bytes) and `data` is set to `nullptr`.
     * 
     * For example:
     * ~~~cpp
     * BLEMIDIPacketBuilder packetbuilder;
     * 
     * const uint8_t *data = (...);
     * size_t length = (...);
     * uint16_t timestamp = (...);
     * 
     * if (!packetbuilder.addSysEx(data, length, timestamp)) {
     *     sendnow(packetbuilder.getBuffer(), packetbuilder.getSize());
     *     packetbuilder.reset();
     *     packetbuilder.addSysEx(data, length, timestamp)
     * }
     * while (data) {
     *     sendnow(packetbuilder.getBuffer(), packetbuilder.getSize());
     *     packetbuilder.reset();
     *     packetbuilder.continueSysEx(data, length, timestamp);
     * }
     * ~~~
     */
    bool addSysEx(const uint8_t *&data, size_t &length, uint16_t timestamp);

    /**
     * @brief   Add a SysEx continuation to the packet.
     * 
     * @param[in,out]   data 
     *                  Pointer to the first byte of the SysEx message to send
     *                  in this continuation packet.
     *                  At the end, this will point to the first byte to send in
     *                  the next packet, or `nullptr` if the message was
     *                  finished.
     * @param[in,out]   length 
     *                  The number of remaining bytes in the SysEx message. 
     *                  At the end, this will be set to remaining number of
     *                  bytes to send in the next packet.
     * @param[in]       timestamp 
     *                  13-bit BLE-MIDI timestamp.
     * 
     * If the message can be completed in a single packet, `length` is set to 
     * `0` (no remaining data bytes) and `data` is set to `nullptr`.
     * 
     * @see @ref addSysEx()
     */
    void continueSysEx(const uint8_t *&data, size_t &length,
                       uint16_t timestamp);
};

END_CS_NAMESPACE
