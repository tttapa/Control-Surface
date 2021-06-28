#pragma once

#include <AH/Error/Error.hpp>

#include "BLEMIDI/BLEMIDIPacketBuilder.hpp"
#include "BLEMIDI/MIDIMessageQueue.hpp"
#include "MIDI_Interface.hpp"
#include "Util/ESP32Threads.hpp"
#include <MIDI_Parsers/BLEMIDIParser.hpp>
#include <MIDI_Parsers/SerialMIDI_Parser.hpp>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

#ifndef ARDUINO
#include <gmock/gmock.h>
#endif

BEGIN_CS_NAMESPACE

/**
 * @brief   Bluetooth Low Energy MIDI Interface for the ESP32.
 * 
 * @ingroup MIDIInterfaces
 */
class BluetoothMIDI_Interface : public MIDI_Interface {

  public:
    BluetoothMIDI_Interface() {
        if (instance)
            FATAL_ERROR(F("Only one instance is supported"), 0x1345);
        instance = this;
    };
    ~BluetoothMIDI_Interface() {
        instance = nullptr;
        stopSendingThread();
        end();
    }

  public:
    /// Send the buffered MIDI BLE packet immediately.
    void flush() {
        lock_t lock(mtx);
        flushImpl(lock);
    }

    /// Set the timeout, the number of milliseconds to buffer the outgoing MIDI
    /// messages. A shorter timeout usually results in lower latency, but also
    /// causes more overhead, because more packets might be required.
    void setTimeout(std::chrono::milliseconds timeout) {
        lock_t lock(mtx);
        this->timeout = timeout;
    }

  public:
    void begin() override;
    void end();

    MIDIReadEvent read();

    void update() override { MIDI_Interface::updateIncoming(this); }

  private:
    void handleStall() override { MIDI_Interface::handleStall(this); }

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
    void sendChannelMessageImpl(ChannelMessage) override;
    void sendSysCommonImpl(SysCommonMessage) override;
    void sendSysExImpl(SysExMessage) override;
    void sendRealTimeImpl(RealTimeMessage) override;
    void sendNowImpl() override { flush(); }

    void sendChannelMessageImpl3Bytes(ChannelMessage);
    void sendChannelMessageImpl2Bytes(ChannelMessage);

  public:
    void parse(const uint8_t *const data, const size_t len);

  private:
    /// The minimum MTU of all connected clients.
    std::atomic_uint_fast16_t min_mtu{23};
    /// Override the minimum MTU (0 means don't override, nonzero overrides if
    /// it's smaller than the minimum MTU of the clients).
    /// @see    @ref forceMinMTU()
    std::atomic_uint_fast16_t force_min_mtu{0};

    /// Set the maximum transmission unit of the Bluetooth link. Used to compute
    /// the MIDI BLE packet size.
    void updateMTU(uint16_t mtu);

  public:
    /// Get the minimum MTU of all connected clients.
    uint16_t getMinMTU() const { return min_mtu; }

    /// Force the MTU to an artificially small value (used for testing).
    void forceMinMTU(uint16_t mtu);

  private:
    /// Only one active instance.
    static BluetoothMIDI_Interface *instance;
    /// MIDI Parser for incoming data.
    SerialMIDI_Parser parser{false};
    /// Builds outgoing MIDI BLE packets.
    BLEMIDIPacketBuilder packetbuilder;
    /// Queue for incoming MIDI messages.
    MIDIMessageQueue queue{64};
    /// Incoming message that can be from retrieved using the
    /// `getChannelMessage()`, `getSysCommonMessage()`, `getRealTimeMessage()`
    /// and `getSysExMessage()` methods.
    MIDIMessageQueue::MIDIMessageQueueElement incomingMessage;

  private:
    // Synchronization for asynchronous BLE sending

    /// Lock type used to lock the mutex
    using lock_t = std::unique_lock<std::mutex>;
    /// Mutex to lock the MIDI BLE packet builder and the flush flag.
    std::mutex mtx;
    /// Condition variable used by the background sender thread to wait for
    /// data to send, and for the main thread to wait for the data to be flushed
    /// by the sender thread.
    std::condition_variable cv;
    /// Background thread that sends the actual MIDI BLE packets.
    std::thread send_thread;
    /// Flag to stop the background thread.
    std::atomic_bool stop_sending{false};
    /// Flag to tell the sender thread to send the packet immediately.
    bool flushnow = false;
    /// Timeout before the sender thread sends a packet.
    /// @see    @ref setTimeout()
    std::chrono::milliseconds timeout{10};

  private:
    /// Launch a thread that sends the BLE packets in the background.
    void startSendingThread();

    /// Function that waits for BLE packets and sends them in the background.
    /// It either sends them after a timeout (a given number of milliseconds
    /// after the first data was added to the packet), or immediately when it
    /// receives a flush signal from the main thread.
    bool handleSendEvents();

    /// Tell the background BLE sender thread to send the current packet.
    /// Blocks until the packet is sent.
    ///
    /// @param  lock
    ///         Lock should be locked at entry, will still be locked on exit.
    void flushImpl(lock_t &lock);

#if !defined(ARDUINO) && !defined(DOXYGEN)
    public:
#endif
    /// Tell the background BLE sender thread to stop gracefully, and join it.
    void stopSendingThread();

  public:
    static void midi_write_callback(const uint8_t *data, size_t length) {
        if (instance)
            instance->parse(data, length);
    }

    static void midi_mtu_callback(uint16_t mtu) {
        if (instance)
            instance->updateMTU(mtu);
    }

#ifdef ARDUINO
  private:
    void notifyMIDIBLE(const std::vector<uint8_t> &packet);
#else
  public:
    MOCK_METHOD(void, notifyMIDIBLE, (const std::vector<uint8_t> &), ());
#endif
};

END_CS_NAMESPACE