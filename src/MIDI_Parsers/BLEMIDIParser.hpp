#pragma once

#include <Settings/NamespaceSettings.hpp>
#include <stddef.h>
#include <stdint.h>

BEGIN_CS_NAMESPACE

/** 
 * @brief   Class for parsing BLE-MIDI packets. It doesn't parse the actual MIDI
 *          messages, it just extracts the relevant MIDI data from the BLE 
 *          packets.
 * 
 * @ingroup MIDIParsers
 */
class BLEMIDIParser {
  public:
    BLEMIDIParser(const uint8_t *data, size_t length)
        : data(data), end(data + length) {
        // Need at least two bytes to be useful.
        // Usually, we have header, timestamp and at least one MIDI byte,
        // but a SysEx continuation could perhaps have only a header and a
        // single data byte (this is not explicitly allowed by the spec, but
        // handling this case requires no extra effort)
        if (length < 2) {
            this->data = end;
        }
        // First byte should be a header. If it's a data byte, discard packet.
        else if (isData(data[0])) {
            this->data = end;
        }
        // If the second byte is a data byte, this is a SysEx continuation
        // packet
        else if (isData(data[1])) {
            this->timestamp = data[0] & 0x7F;
            this->timestamp <<= 7;
            this->data += 1;
        }
        // Otherwise, the second byte is a timestamp, so skip it
        else {
            this->timestamp = data[0] & 0x7F;
            this->timestamp <<= 7;
            this->timestamp |= data[1] & 0x7F;
            this->data += 2;
        }
    }

    /// Get the next MIDI byte from the BLE packet (if available).
    /// @return True if a byte was available, false otherwise.
    bool pull(uint8_t &output) {
        while (data != end) {
            // Simply pass on all normal data bytes to the MIDI parser.
            if (isData(*data)) {
                output = *data++;
                prevWasTimestamp = false;
                return true;
            }
            // If it's not a data byte, it's either a timestamp byte or a
            // MIDI status byte.
            else {
                // Timestamp bytes and MIDI status bytes should alternate.
                prevWasTimestamp = !prevWasTimestamp;
                // If the previous non-data byte was a timestamp, this one is
                // a MIDI status byte, so send it to the MIDI parser.
                if (!prevWasTimestamp) {
                    output = *data++;
                    return true;
                }
                // Otherwise it's a time stamp
                else {
                    timestamp = (timestamp & 0x3F80) | (*data++ & 0x7F);
                }
            }
        }
        return false;
    }

    uint16_t getTimestamp() const { return timestamp; }

  private:
    const uint8_t *data;
    const uint8_t *const end;
    bool prevWasTimestamp = true;
    uint16_t timestamp = 0;

  private:
    /// Check if the given byte is a data byte (and not a header, timestamp or
    /// status byte).
    static bool isData(uint8_t data) { return (data & (1 << 7)) == 0; }
};

END_CS_NAMESPACE
