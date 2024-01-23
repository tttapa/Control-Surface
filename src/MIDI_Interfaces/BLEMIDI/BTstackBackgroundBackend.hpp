#pragma once

#include <AH/Error/Error.hpp>

#include "BLEAPI.hpp"
#include "BTstack/gatt_midi.hpp"
#include "BufferedBLEMIDIParser.hpp"
#include "PollingBLEMIDISender.hpp"

#include <atomic>
#include <csignal>
#include <hardware/sync.h>

BEGIN_CS_NAMESPACE

/// Raspberry Pi Pico BTstack background backend intended to be plugged into
/// @ref GenericBLEMIDI_Interface.
///
/// @todo   Implement BTstack timer-based sender timeouts.
class BTstackBackgroundBackend
    : private PollingBLEMIDISender<BTstackBackgroundBackend>,
      private MIDIBLEInstance {
  private:
    // Callbacks from the ArduinoBLE stack.
    void handleConnect(BLEConnectionHandle conn_handle) override {
        connected = conn_handle.conn;
    }
    void handleDisconnect(BLEConnectionHandle) override {
        connected = subscribed = 0xFFFF;
    }
    void handleMTU(BLEConnectionHandle, uint16_t mtu) override {
        Sender::updateMTU(mtu);
    }
    void handleSubscribe(BLEConnectionHandle,
                         BLECharacteristicHandle char_handle,
                         bool notify) override {
        subscribed = notify ? char_handle.characteristic : 0xFFFF;
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
    // We cannot use atomics here, because they might not be lock-free on the
    // Pico's Cortex-M0+ cores.
    static_assert(sizeof(sig_atomic_t) > sizeof(uint16_t));
    /// Are we connected to a BLE Central?
    volatile sig_atomic_t connected = 0xFFFF;
    /// Did the BLE Central subscribe to be notified for the MIDI characteristic?
    volatile sig_atomic_t subscribed = 0xFFFF;
    struct VolatileSize {
        constexpr static size_t alignment = alignof(sig_atomic_t);
        VolatileSize(sig_atomic_t value) : value {value} {}
        volatile sig_atomic_t value;
        sig_atomic_t load_acquire() const {
            auto state = save_and_disable_interrupts(); // Probably overkill ...
            auto t = value;
            restore_interrupts(state);
            std::atomic_signal_fence(std::memory_order_acquire);
            return t;
        }
        void add_release(sig_atomic_t t) {
            std::atomic_signal_fence(std::memory_order_release);
            auto state = save_and_disable_interrupts();
            value += t;
            restore_interrupts(state);
        }
        void sub_release(sig_atomic_t t) {
            std::atomic_signal_fence(std::memory_order_release);
            auto state = save_and_disable_interrupts();
            value -= t;
            restore_interrupts(state);
        }
    };
    /// Contains incoming BLE MIDI data to be parsed.
    BufferedBLEMIDIParser<1024, VolatileSize> parser;

  public:
    using IncomingMIDIMessage = AnyMIDIMessage;

    /// Retrieve and remove a single incoming MIDI message from the buffer.
    bool popMessage(IncomingMIDIMessage &incomingMessage) {
        // This function is assumed to be polled regularly by the higher-level
        // MIDI_Interface, so we check the sender's timer here.
        auto lck = Sender::acquirePacket();
        Sender::releasePacketAndNotify(lck);
        // Actually get a MIDI message from the buffer
        return parser.popMessage(incomingMessage);
    }

  public:
    /// Initialize the BLE stack etc.
    void begin(BLESettings ble_settings) {
        midi_ble_btstack::init(*this, ble_settings);
        Sender::begin();
    }
    /// Deinitialize the BLE stack.
    /// @todo   Not yet implemented.
    void end() {}
    /// Returns true if we are connected to a BLE Central device.
    bool isConnected() const { return connected != 0xFFFF; }

  private:
    // Implement the interface for the BLE sender.
    using Sender = PollingBLEMIDISender<BTstackBackgroundBackend>;
    friend Sender;
    /// Send the given MIDI BLE packet.
    void sendData(BLEDataView data) {
        if (connected != 0xFFFF && subscribed != 0xFFFF)
            midi_ble_btstack::notify(
                BLEConnectionHandle {static_cast<uint16_t>(connected)},
                BLECharacteristicHandle {static_cast<uint16_t>(subscribed)},
                data);
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