#pragma once

#include <AH/Error/Error.hpp>

#include "BLEMIDI/MIDIMessageQueue.hpp"
#include "MIDI_Interface.hpp"
#include "Util/ESP32Threads.hpp"
#include <MIDI_Parsers/BLEMIDIParser.hpp>
#include <MIDI_Parsers/SerialMIDI_Parser.hpp>

#include "BLEMIDI/BLEAPI.hpp"                                // TODO
#include "BLEMIDI/ESP32-NimBLE/app.hpp"                      // TODO
#include "MIDI_Interfaces/BLEMIDI/FreeRTOSBLEMIDISender.hpp" // TODO

#include <chrono>

BEGIN_CS_NAMESPACE

// TODO: move to separate file
struct MIDIBLEBackend : private FreeRTOSBLEMIDISender<MIDIBLEBackend>,
                        private MIDIBLEInstance {
  private:
    using Sender = FreeRTOSBLEMIDISender<MIDIBLEBackend>;
    friend Sender;
    void sendData(BLEDataView data) {
        auto chr = characteristic.load();
        auto con = connection.load();
        ESP_LOGI("CS-BLEMIDI", "conn=%d, char=%d", con.conn,
                 chr.characteristic);
        if (chr.characteristic == 0xFFFF)
            return;
        midi_ble::notify(con, chr, data);
    }
    std::atomic<BLEConnectionHandle> connection;
    std::atomic<BLECharacteristicHandle> characteristic;

  private:
    void handleConnect(BLEConnectionHandle conn_handle) override {
        ESP_LOGI("CS-BLEMIDI", "conn=%d", conn_handle.conn);
        this->connection.store(conn_handle);
    }
    void handleDisconnect(BLEConnectionHandle conn_handle) override {
        ESP_LOGI("CS-BLEMIDI", "conn=%d", conn_handle.conn);
        this->connection.store({});
        this->characteristic.store({});
    }
    void handleMTU(BLEConnectionHandle conn_handle, uint16_t mtu) override {
        ESP_LOGI("CS-BLEMIDI", "conn=%d, mtu=%d", conn_handle.conn, mtu);
        Sender::updateMTU(mtu);
    }
    void handleSubscribe(BLEConnectionHandle conn_handle,
                         BLECharacteristicHandle char_handle,
                         bool notify) override {
        ESP_LOGI("CS-BLEMIDI", "conn=%d, char=%d, notify=%d", conn_handle.conn,
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
        ESP_LOGI("CS-BLEMIDI", "conn=%d", conn_handle.conn);
    }

  public:
    void begin(BLESettings ble_settings) {
        midi_ble::init(*this, ble_settings);
        // Need larger stack than default, pin to non-Arduino core
        ScopedThreadConfig sc {4096, 3, true, "CS-BLEMIDI", 0};
        Sender::begin();
    }
    using Sender::acquirePacket;
    using Sender::forceMinMTU;
    using Sender::getMinMTU;
    using Sender::releasePacketAndNotify;
    using Sender::sendNow;
    using Sender::setTimeout;
};

/**
 * @brief   Bluetooth Low Energy MIDI Interface for the ESP32.
 * 
 * @ingroup MIDIInterfaces
 */
class GenericBLEMIDI_Interface : public MIDI_Interface {
    using Backend = MIDIBLEBackend; // TODO

  public:
    Backend backend;

    template <class... Args>
    GenericBLEMIDI_Interface(Args &&...args)
        : backend {std::forward<Args>(args)...} {}

  public:
    /// Set the BLE device name. Must be called before @ref begin().
    void setName(const char *name);

    void begin() override;
    void end();

    MIDIReadEvent read();

    void update() override { MIDI_Interface::updateIncoming(this); }

    /// Set the timeout, the number of milliseconds to buffer the outgoing MIDI
    /// messages. A shorter timeout usually results in lower latency, but also
    /// causes more overhead, because more packets might be required.
    void setTimeout(std::chrono::milliseconds timeout) {
        backend.setTimeout(timeout);
    }

    BLESettings ble_settings;

  private:
#if !DISABLE_PIPES
    void handleStall() override { MIDI_Interface::handleStall(this); }
#endif

  public:
    /// Return the received channel voice message.
    ChannelMessage getChannelMessage() const;
    /// Return the received system common message.
    SysCommonMessage getSysCommonMessage() const;
    /// Return the received real-time message.
    RealTimeMessage getRealTimeMessage() const;
    /// Return the received system exclusive message.
    SysExMessage getSysExMessage() const;
    /// Get the BLE-MIDI timestamp of the latest MIDI message.
    /// @note Invalid for SysEx chunks (except the last chunk of a message).
    uint16_t getTimestamp() const;

  protected:
    // MIDI send implementations
    template <class L, class F>
    void sendImpl(L &lck, F add_to_buffer);
    void sendChannelMessageImpl(ChannelMessage) override;
    void sendSysCommonImpl(SysCommonMessage) override;
    void sendSysExImpl(SysExMessage) override;
    void sendRealTimeImpl(RealTimeMessage) override;
    /// Send the buffered MIDI BLE packet immediately.
    void sendNowImpl() override {
        auto lck = backend.acquirePacket();
        backend.sendNow(lck);
    }

  public:
    /// Parse the given BLE data immediately, in a thread-safe way, and push the
    /// results to the MIDI message queue.
    void parseNowThreadSafe(BLEDataGenerator ble_data);

  private:
    /// May contain incoming data to be parsed.
    BLEDataGenerator ble_data {};
    /// Parses the (chunked) BLE packet obtained from @ref readdata.
    BLEMIDIParser ble_parser {nullptr, 0};
    /// MIDI Parser for incoming data.
    SerialMIDI_Parser parser {false};
    /// Queue for incoming MIDI messages.
    MIDIMessageQueue queue {64};
    /// Incoming message that can be from retrieved using the
    /// `getChannelMessage()`, `getSysCommonMessage()`, `getRealTimeMessage()`
    /// and `getSysExMessage()` methods.
    MIDIMessageQueue::MIDIMessageQueueElement incomingMessage;
    std::atomic_uint queue_full_count {0};

    void queueFull() {
        queue_full_count.fetch_add(1, std::memory_order_relaxed);
    }
};

END_CS_NAMESPACE