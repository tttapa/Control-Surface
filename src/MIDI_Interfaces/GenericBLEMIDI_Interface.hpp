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

class MIDIBLEParser {
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

// TODO: move to separate file
class MIDIBLEBackend : private FreeRTOSBLEMIDISender<MIDIBLEBackend>,
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
        parser.parseNowThreadSafe(std::move(data));
    }

  private:
    MIDIBLEParser parser;

  public:
    using IncomingMIDIMessage = MIDIBLEParser::IncomingMIDIMessage;
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
    void sendNowImpl() override;

  private:
    /// Incoming message that can be from retrieved using the
    /// `getChannelMessage()`, `getSysCommonMessage()`, `getRealTimeMessage()`
    /// and `getSysExMessage()` methods.
    Backend::IncomingMIDIMessage incomingMessage;
};

END_CS_NAMESPACE