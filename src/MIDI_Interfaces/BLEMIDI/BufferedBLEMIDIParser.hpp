#pragma once

#include <Settings/NamespaceSettings.hpp>

#include "BLERingBuf.hpp"
#include <MIDI_Parsers/AnyMIDI_Message.hpp>
#include <MIDI_Parsers/BLEMIDIParser.hpp>
#include <MIDI_Parsers/SerialMIDI_Parser.hpp>

BEGIN_CS_NAMESPACE

/// FIFO buffer that you can push BLE packets into, and pop MIDI messages out of.
/// If @p SizeT is chosen to be atomic, one thread can push packets, and another
/// thread can pop MIDI messages, without additional synchronization.
template <uint16_t Capacity, class SizeT = NonatomicBLERingBufSize<uint16_t>>
class BufferedBLEMIDIParser {
  private:
    /// Contains incoming data to be parsed.
    BLERingBuf<Capacity, SizeT> ble_buffer {};
    /// Parses the (chunked) BLE packet obtained from @ref ble_buffer.
    BLEMIDIParser ble_parser {nullptr, 0};
    /// Parser for MIDI data extracted from the BLE packet by @ref ble_parser.
    SerialMIDI_Parser parser {false};

  public:
    using IncomingMIDIMessage = AnyMIDIMessage;

    /// Add a new BLE packet or chunk to the buffer.
    bool pushPacket(BLEDataView packet,
                    BLEDataType type = BLEDataType::Packet) {
        return ble_buffer.push(packet, type);
    }

    /// Retrieve and remove a single incoming MIDI message from the buffer.
    bool popMessage(IncomingMIDIMessage &incomingMessage) {
        // Try reading a MIDI message from the parser
        auto try_read = [&] {
            MIDIReadEvent event = parser.pull(ble_parser);
            switch (event) {
                case MIDIReadEvent::CHANNEL_MESSAGE:
                    incomingMessage = {parser.getChannelMessage(),
                                       ble_parser.getTimestamp()};
                    return true;
                case MIDIReadEvent::SYSEX_CHUNK: // fallthrough
                case MIDIReadEvent::SYSEX_MESSAGE:
                    incomingMessage = {parser.getSysExMessage(),
                                       ble_parser.getTimestamp()};
                    return true;
                case MIDIReadEvent::REALTIME_MESSAGE:
                    incomingMessage = {parser.getRealTimeMessage(),
                                       ble_parser.getTimestamp()};
                    return true;
                case MIDIReadEvent::SYSCOMMON_MESSAGE:
                    incomingMessage = {parser.getSysCommonMessage(),
                                       ble_parser.getTimestamp()};
                    return true;
                case MIDIReadEvent::NO_MESSAGE: return false;
                default: break; // LCOV_EXCL_LINE
            }
            return false;
        };
        while (true) {
            // Try reading a MIDI message from the current buffer
            if (try_read())
                return true; // success, incomingMessage updated
            // Get the next chunk of the BLE packet (if available)
            BLEDataView chunk;
            auto popped = ble_buffer.pop(chunk);
            if (popped == BLEDataType::None)
                return false; // no more BLE data available
            else if (popped == BLEDataType::Continuation)
                ble_parser.extend(chunk.data, chunk.length); // same BLE packet
            else if (popped == BLEDataType::Packet)
                ble_parser = {chunk.data, chunk.length}; // new BLE packet
        }
    }
};

END_CS_NAMESPACE