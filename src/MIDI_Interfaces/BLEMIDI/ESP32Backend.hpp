#pragma once

#include <AH/Error/Error.hpp>

#include "BLEAPI.hpp"
#include "BufferedBLEMIDIParser.hpp"
#include "ThreadedBLEMIDISender.hpp"
#include "Util/ESP32Threads.hpp"

#include <atomic>

#ifndef ARDUINO
#define ESP_LOGD(...) ((void)0)
#define ESP_LOGE(...) ((void)0)
#define ESP_LOGI(...) ((void)0)
#endif

BEGIN_CS_NAMESPACE

/// ESP32 backend intended to be plugged into @ref GenericBLEMIDI_Interface.
/// @p Impl can be used to select different low-level BLE stacks.
template <class Impl>
class ESP32BLEBackend : private ThreadedBLEMIDISender<ESP32BLEBackend<Impl>>,
                        private MIDIBLEInstance {
  protected:
    [[no_unique_address]] Impl impl;
    using Sender = ThreadedBLEMIDISender<ESP32BLEBackend>;
    friend Sender;
    void sendData(BLEDataView data) {
        auto chr = characteristic.load();
        auto con = connection.load();
        ESP_LOGD("CS-BLEMIDI", "conn=%d, char=%d", con.conn,
                 chr.characteristic);
        if (chr.characteristic == 0xFFFF)
            return;
        impl.notify(con, chr, data);
    }
    std::atomic<BLEConnectionHandle> connection;
    std::atomic<BLECharacteristicHandle> characteristic;

  protected:
    void handleConnect(BLEConnectionHandle conn_handle) override {
        ESP_LOGD("CS-BLEMIDI", "conn=%d", conn_handle.conn);
        this->connection.store(conn_handle);
    }
    void handleDisconnect(
        [[maybe_unused]] BLEConnectionHandle conn_handle) override {
        ESP_LOGD("CS-BLEMIDI", "conn=%d", conn_handle.conn);
        this->connection.store({});
        this->characteristic.store({});
    }
    void handleMTU([[maybe_unused]] BLEConnectionHandle conn_handle,
                   uint16_t mtu) override {
        ESP_LOGD("CS-BLEMIDI", "conn=%d, mtu=%d", conn_handle.conn, mtu);
        Sender::updateMTU(mtu);
    }
    void handleSubscribe(BLEConnectionHandle conn_handle,
                         BLECharacteristicHandle char_handle,
                         bool notify) override {
        ESP_LOGD("CS-BLEMIDI", "conn=%d, char=%d, notify=%d", conn_handle.conn,
                 char_handle.characteristic, +notify);
        if (notify) {
            this->connection.store(conn_handle);
            this->characteristic.store(char_handle);
        } else {
            this->characteristic.store({});
        }
    }
    void handleData([[maybe_unused]] BLEConnectionHandle conn_handle,
                    BLEDataGenerator &&data, BLEDataLifetime) override {
        ESP_LOGD("CS-BLEMIDI", "conn=%d", conn_handle.conn);
        BLEDataView packet = data();
        if (!packet)
            return;
        if (!parser.pushPacket(packet)) {
            ESP_LOGE("CS-BLEMIDI", "BLE packet dropped, size=%d",
                     packet.length);
            return;
        }
        while (BLEDataView cont = data()) {
            if (!parser.pushPacket(cont, BLEDataType::Continuation)) {
                ESP_LOGE("CS-BLEMIDI", "BLE chunk dropped, size=%d",
                         cont.length);
                return;
            } else {
                ESP_LOGI("CS-BLEMIDI", "added chunk, size=%d", cont.length);
            }
        }
    }

  private:
    struct AtomicSize {
#ifdef ESP32
        constexpr static size_t alignment = 32; // default cache size
#else
        constexpr static size_t alignment = 64;
#endif
        AtomicSize(uint_fast16_t value) : value {value} {}
        std::atomic_uint_fast16_t value;
        uint_fast16_t load_acquire() const {
            return value.load(std::memory_order_acquire);
        }
        void add_release(uint_fast16_t t) {
            value.fetch_add(t, std::memory_order_release);
        }
        void sub_release(uint_fast16_t t) {
            value.fetch_sub(t, std::memory_order_release);
        }
    };
    /// Contains incoming BLE MIDI data to be parsed.
    BufferedBLEMIDIParser<4096, AtomicSize> parser;

  public:
    using IncomingMIDIMessage = AnyMIDIMessage;
    bool popMessage(IncomingMIDIMessage &incomingMessage) {
        return parser.popMessage(incomingMessage);
    }

  public:
    void begin(BLESettings ble_settings) {
        impl.init(*this, ble_settings);
        // Need larger stack than default, pin to non-Arduino core
        ScopedThreadConfig sc {4096, 3, true, "CS-BLEMIDI", 0};
        Sender::begin();
    }
    void end() {
        FATAL_ERROR(F("ESP32BLEBackend::end not implemented"), 0x3278);
    }
    bool isConnected() const {
        return connection.load(std::memory_order_relaxed).conn != 0xFFFF;
    }
    using Sender::acquirePacket;
    using Sender::forceMinMTU;
    using Sender::getMinMTU;
    using Sender::releasePacketAndNotify;
    using Sender::sendNow;
    using Sender::setTimeout;
};

END_CS_NAMESPACE