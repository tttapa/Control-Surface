#pragma once

#include <AH/Error/Error.hpp>

#include "MIDIMessageQueue.hpp"
#include <MIDI_Parsers/BLEMIDIParser.hpp>
#include <MIDI_Parsers/SerialMIDI_Parser.hpp>

#include "BLEAPI.hpp"

BEGIN_CS_NAMESPACE

/// Class that parses BLE MIDI data and pushes the resulting messages into a
/// thread-safe SPSC queue.
/// It is intended to be used from the BLE stack callback (which runs in a
/// separate FreeRTOS task).
/// @note   SysEx messages are dynamically allocated using `new`.
/// @note   If the queue is full, messages will be dropped.
class ThreadSafeMIDIBLEParser {
  public:
    /// Parse the given BLE data immediately, in a thread-safe way, and push the
    /// results to the MIDI message queue.
    void parseNowThreadSafe(BLEDataGenerator ble_data) {
        auto chunk = ble_data();
        if (chunk.length == 0)
            return;
        ble_parser = {chunk.data, chunk.length};
        while (true) {
            MIDIReadEvent event = parser.pull(ble_parser);
            while (event != MIDIReadEvent::NO_MESSAGE) {
                switch (event) {
                    case MIDIReadEvent::CHANNEL_MESSAGE:
                        if (!queue.push(parser.getChannelMessage(),
                                        ble_parser.getTimestamp()))
                            queueFull();
                        break;
                    case MIDIReadEvent::SYSEX_CHUNK: // fallthrough
                    case MIDIReadEvent::SYSEX_MESSAGE:
                        if (!queue.push(parser.getSysExMessage(),
                                        ble_parser.getTimestamp()))
                            queueFull();
                        break;
                    case MIDIReadEvent::REALTIME_MESSAGE:
                        if (!queue.push(parser.getRealTimeMessage(),
                                        ble_parser.getTimestamp()))
                            queueFull();
                        break;
                    case MIDIReadEvent::SYSCOMMON_MESSAGE:
                        if (!queue.push(parser.getSysCommonMessage(),
                                        ble_parser.getTimestamp()))
                            queueFull();
                        break;
                    case MIDIReadEvent::NO_MESSAGE: break; // LCOV_EXCL_LINE
                    default: break;                        // LCOV_EXCL_LINE
                }
                event = parser.pull(ble_parser);
            }
            chunk = ble_data();
            if (chunk.length == 0)
                break;
            ble_parser.extend(chunk.data, chunk.length);
        }
        parser.cancelRunningStatus();
    }

    using IncomingMIDIMessage = MIDIMessageQueue::MIDIMessageQueueElement;

    bool popMessage(IncomingMIDIMessage &incomingMessage) {
        return queue.pop(incomingMessage);
    }

    unsigned getNumDroppedMessages() const {
        return queue_full_count.load(std::memory_order_relaxed);
    }

  private:
    /// May contain incoming data to be parsed.
    BLEDataGenerator ble_data {};
    /// Parses the (chunked) BLE packet obtained from @ref readdata.
    BLEMIDIParser ble_parser {nullptr, 0};
    /// MIDI Parser for incoming data.
    SerialMIDI_Parser parser {false};
    /// Queue for incoming MIDI messages.
    MIDIMessageQueue queue {64};
    /// The number of times a MIDI message was dropped because the queue was
    /// full.
    std::atomic_uint queue_full_count {0};

    void queueFull() {
        queue_full_count.fetch_add(1, std::memory_order_relaxed);
    }
};

END_CS_NAMESPACE