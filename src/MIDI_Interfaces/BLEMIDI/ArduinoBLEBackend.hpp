#pragma once

#include <AH/Error/Error.hpp>

#include "ArduinoBLE/midi.hpp"
#include "BLEAPI.hpp"
#include "BufferedBLEMIDIParser.hpp"
#include "PollingBLEMIDISender.hpp"

BEGIN_CS_NAMESPACE

/// ArduinoBLE backend intended to be plugged into
/// @ref GenericBLEMIDI_Interface.
class ArduinoBLEBackend : private PollingBLEMIDISender<ArduinoBLEBackend>,
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
            } else if (!parser.pushPacket(packet)) {
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
    /// Contains incoming BLE MIDI data to be parsed.
    BufferedBLEMIDIParser<1024> parser;

  public:
    using IncomingMIDIMessage = AnyMIDIMessage;

    /// Retrieve and remove a single incoming MIDI message from the buffer.
    bool popMessage(IncomingMIDIMessage &incomingMessage) {
        // This function is assumed to be polled regularly by the higher-level
        // MIDI_Interface, so we check the sender's timer here, and we poll
        // the ArduinoBLE library.
        auto lck = Sender::acquirePacket();
        Sender::releasePacketAndNotify(lck);
        arduino_ble_midi::poll();
        // Actually get a MIDI message from the buffer
        return parser.popMessage(incomingMessage);
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
    using Sender = PollingBLEMIDISender<ArduinoBLEBackend>;
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