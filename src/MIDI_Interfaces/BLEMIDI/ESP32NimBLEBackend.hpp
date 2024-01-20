#pragma once

#include <AH/Error/Error.hpp>

#include "BLEAPI.hpp"
#include "ESP32-NimBLE/app.hpp"
#include "FreeRTOSBLEMIDISender.hpp"
#include "ThreadSafeMIDIBLEParser.hpp"
#include "Util/ESP32Threads.hpp"

BEGIN_CS_NAMESPACE

/// ESP32 NimBLE backend intended to be plugged into
/// @ref GenericBLEMIDI_Interface.
class ESP32NimBLEBackend : private FreeRTOSBLEMIDISender<ESP32NimBLEBackend>,
                           private MIDIBLEInstance {
  private:
    using Sender = FreeRTOSBLEMIDISender<ESP32NimBLEBackend>;
    friend Sender;
    void sendData(BLEDataView data) {
        auto chr = characteristic.load();
        auto con = connection.load();
        ESP_LOGD("CS-BLEMIDI", "conn=%d, char=%d", con.conn,
                 chr.characteristic);
        if (chr.characteristic == 0xFFFF)
            return;
        midi_ble::notify(con, chr, data);
    }
    std::atomic<BLEConnectionHandle> connection;
    std::atomic<BLECharacteristicHandle> characteristic;

  private:
    void handleConnect(BLEConnectionHandle conn_handle) override {
        ESP_LOGD("CS-BLEMIDI", "conn=%d", conn_handle.conn);
        this->connection.store(conn_handle);
    }
    void handleDisconnect(BLEConnectionHandle conn_handle) override {
        ESP_LOGD("CS-BLEMIDI", "conn=%d", conn_handle.conn);
        this->connection.store({});
        this->characteristic.store({});
    }
    void handleMTU(BLEConnectionHandle conn_handle, uint16_t mtu) override {
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
    void handleData(BLEConnectionHandle conn_handle, BLEDataGenerator &&data,
                    BLEDataLifetime lifetime) override {
        ESP_LOGD("CS-BLEMIDI", "conn=%d", conn_handle.conn);
        parser.parseNowThreadSafe(std::move(data));
    }

  private:
    ThreadSafeMIDIBLEParser parser;

  public:
    using IncomingMIDIMessage = ThreadSafeMIDIBLEParser::IncomingMIDIMessage;
    bool popMessage(IncomingMIDIMessage &incomingMessage) {
        return parser.popMessage(incomingMessage);
    }

  public:
    void begin(BLESettings ble_settings) {
        midi_ble::init(*this, ble_settings);
        // Need larger stack than default, pin to non-Arduino core
        ScopedThreadConfig sc {4096, 3, true, "CS-BLEMIDI", 0};
        Sender::begin();
    }
    void end() {
        // TODO
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