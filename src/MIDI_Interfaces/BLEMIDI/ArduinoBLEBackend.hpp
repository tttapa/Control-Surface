#pragma once

#include <AH/Error/Error.hpp>

#include "ArduinoBLE/midi.hpp"
#include "BLEAPI.hpp"
#include "MIDI_Interfaces/BLEMIDI/BLERingBuf.hpp"
#include "PollingMIDISender.hpp"
#include <MIDI_Parsers/BLEMIDIParser.hpp>
#include <MIDI_Parsers/SerialMIDI_Parser.hpp>

BEGIN_CS_NAMESPACE

/// ArduinoBLE backend intended to be plugged into
/// @ref GenericBLEMIDI_Interface.
class ArduinoBLEBackend : private PollingMIDISender<ArduinoBLEBackend>,
                          private MIDIBLEInstance {
  private:
    // Callbacks from the ArduinoBLE stack.
    void handleConnect(BLEConnectionHandle) override { connected = true; }
    void handleDisconnect(BLEConnectionHandle) override {
        connected = subscribed = false;
    }
    void handleMTU(BLEConnectionHandle, uint16_t mtu) override {
        Sender::updateMTU(mtu);
    }
    void handleSubscribe(BLEConnectionHandle, BLECharacteristicHandle,
                         bool notify) override {
        subscribed = notify;
    }
    void handleData(BLEConnectionHandle, BLEDataGenerator &&data,
                    BLEDataLifetime) override {
        while (true) {
            BLEDataView packet = data();
            if (packet.length == 0) {
                break;
            } else if (!ble_buffer.push(packet)) {
                DEBUGREF(F("BLE packet dropped, size: ") << packet.length);
                break;
            }
        }
    }

  private:
    /// Are we connected to a BLE Central?
    bool connected = false;
    /// Did the BLE Central subscribe to be notified for the MIDI characteristic?
    bool subscribed = false;
    /// Contains incoming data to be parsed.
    BLERingBuf<1024> ble_buffer {};
    /// Parses the (chunked) BLE packet obtained from @ref ble_buffer.
    BLEMIDIParser ble_parser {nullptr, 0};
    /// Parser for MIDI data extracted from the BLE packet by @ref ble_parser.
    SerialMIDI_Parser parser {false};

  public:
    /// MIDI message variant type.
    struct IncomingMIDIMessage {
        MIDIReadEvent eventType = MIDIReadEvent::NO_MESSAGE;
        union Message {
            ChannelMessage channelmessage;
            SysCommonMessage syscommonmessage;
            RealTimeMessage realtimemessage;
            SysExMessage sysexmessage;

            Message() : realtimemessage(0x00) {}
            Message(ChannelMessage msg) : channelmessage(msg) {}
            Message(SysCommonMessage msg) : syscommonmessage(msg) {}
            Message(RealTimeMessage msg) : realtimemessage(msg) {}
            Message(SysExMessage msg) : sysexmessage(msg) {}
        } message;
        uint16_t timestamp = 0xFFFF;

        IncomingMIDIMessage() = default;
        IncomingMIDIMessage(ChannelMessage message, uint16_t timestamp)
            : eventType(MIDIReadEvent::CHANNEL_MESSAGE), message(message),
              timestamp(timestamp) {}
        IncomingMIDIMessage(SysCommonMessage message, uint16_t timestamp)
            : eventType(MIDIReadEvent::SYSCOMMON_MESSAGE), message(message),
              timestamp(timestamp) {}
        IncomingMIDIMessage(RealTimeMessage message, uint16_t timestamp)
            : eventType(MIDIReadEvent::REALTIME_MESSAGE), message(message),
              timestamp(timestamp) {}
        IncomingMIDIMessage(SysExMessage message, uint16_t timestamp)
            : eventType(message.isLastChunk() ? MIDIReadEvent::SYSEX_MESSAGE
                                              : MIDIReadEvent::SYSEX_CHUNK),
              message(message), timestamp(timestamp) {}
    };

    /// Retrieve and remove a single incoming MIDI message from the buffer.
    bool popMessage(IncomingMIDIMessage &incomingMessage) {
        // This function is assumed to be polled regularly by the higher-level
        // MIDI_Interface, so we check the sender's timer here, and we poll
        // the ArduinoBLE library.
        auto lck = Sender::acquirePacket();
        Sender::releasePacketAndNotify(lck);
        arduino_ble_midi::poll();
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

  public:
    /// Initialize the BLE stack etc.
    void begin(BLESettings ble_settings) {
        arduino_ble_midi::init(*this, ble_settings);
        Sender::begin();
    }
    /// Deinitialize the BLE stack.
    /// @todo   Not yet implemented.
    void end() {}
    /// Returns true if we are connected to a BLE Central device.
    bool isConnected() const { return connected; }

  private:
    // Implement the interface for the BLE sender.
    using Sender = PollingMIDISender<ArduinoBLEBackend>;
    friend Sender;
    /// Send the given MIDI BLE packet.
    void sendData(BLEDataView data) {
        if (connected && subscribed)
            arduino_ble_midi::notify(data);
    }

  public:
    // Expose the necessary BLE sender functions.
    using Sender::acquirePacket;
    using Sender::forceMinMTU;
    using Sender::getMinMTU;
    using Sender::releasePacketAndNotify;
    using Sender::sendNow;
    using Sender::setTimeout;
};

END_CS_NAMESPACE