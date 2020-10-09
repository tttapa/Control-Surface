#pragma once

#include <AH/Error/Error.hpp>

#include "BLEMIDI/BLEMIDI.hpp"
#include "BLEMIDI/BLEMIDI_Parser.hpp"
#include "BLEMIDI/BLEMIDIPacketBuilder.hpp"
#include "MIDI_Interface.hpp"
#include <MIDI_Parsers/SerialMIDI_Parser.hpp>
#include "Util/ESP32Threads.hpp"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

BEGIN_CS_NAMESPACE

/**
 * @brief   Bluetooth Low Energy MIDI Interface for the ESP32.
 * 
 * @ingroup MIDIInterfaces
 */
class BluetoothMIDI_Interface : public MIDI_Interface,
                                public BLEServerCallbacks,
                                public BLECharacteristicCallbacks {

  public:
    BluetoothMIDI_Interface() = default;
    ~BluetoothMIDI_Interface() { stopSendingThread(); }

  private:
    /// BLE connection manager.
    BLEMIDI bleMidi;
    /// MIDI Parser for incoming data.
    SerialMIDI_Parser parser;
    /// Builds outgoing MIDI BLE packets.
    BLEMIDIPacketBuilder packetbuilder;

  public:
    /// For tests.
    BLEMIDI &getBLEMIDI() { return bleMidi; }

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

    /// Launch a thread that sends the BLE packets in the background.
    void startSendingThread();

    /// Function that waits for BLE packets and sends them in the background.
    /// It either sends them after a timeout (a given number of milliseconds
    /// after the first data was added to the packet), or immediately when it
    /// receives a flush signal from the main thread.
    void handleSendEvents();

    /// Tell the background BLE sender thread to send the current packet.
    /// Blocks until the packet is sent. 
    ///
    /// @param  lock 
    ///         Lock should be locked at entry, will still be locked on exit.
    void flushImpl(lock_t &lock);

    /// Tell the background BLE sender thread to stop gracefully, and join it.
    void stopSendingThread();

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
    void begin() override {
        bleMidi.begin(this, this);
        startSendingThread();
    }

    void update() override {
        // TODO: dispatch MIDI events in main thread
    }

  protected:
    // MIDI send implementations
    void sendImpl(uint8_t header, uint8_t d1, uint8_t d2, uint8_t cn) override;
    void sendImpl(uint8_t header, uint8_t d1, uint8_t cn) override;
    void sendImpl(const uint8_t *data, size_t length, uint8_t cn) override;
    void sendImpl(uint8_t rt, uint8_t cn) override;

  private:
    void handleStall() override {
        auto stallername = MIDIStaller::getNameNull(getStaller());
        ERROR(F("Not implemented (stalled by ") << stallername << ')', 0x1349);
        (void)stallername;
    }

  public:
    void parse(const uint8_t *const data, const size_t len) {
        auto midiparser = [this](uint8_t databyte) { parse(databyte); };
        BLEMIDI_Parser(midiparser)(data, len);
    }

    /// Return the received channel message.
    ChannelMessage getChannelMessage() const {
        return parser.getChannelMessage();
    }
    /// Return the received real-time message.
    RealTimeMessage getRealTimeMessage() const {
        return parser.getRealTimeMessage();
    }
    /// Return the received system exclusive message.
    SysExMessage getSysExMessage() const { return parser.getSysExMessage(); }

  private:
    void parse(uint8_t data) {
        MIDIReadEvent event = parser.parse(data);
        // Best we can do is just retry until the pipe is no longer in exclusive
        // mode.
        // Keep in mind that this is executed in the callback of the BLE stack,
        // I don't know what happens to the Bluetooth connection if we let it
        // wait for longer than the communication interval.
        //
        // TODO: If this causes problems, we could buffer the data until the
        //       pipe is available for writing again.
        while (!dispatchMIDIEvent(event))
#ifdef ARDUINO
            delay(1);
#else
            usleep(1e3);
#endif

        // TODO: use a queue to the main thread
    }

    bool dispatchMIDIEvent(MIDIReadEvent event) {
        switch (event) {
            case MIDIReadEvent::NO_MESSAGE: 
                return true;
            case MIDIReadEvent::CHANNEL_MESSAGE:
                return onChannelMessage(getChannelMessage());
            case MIDIReadEvent::SYSEX_MESSAGE: 
                return onSysExMessage(getSysExMessage());
            case MIDIReadEvent::REALTIME_MESSAGE: 
                return onRealTimeMessage(getRealTimeMessage());
            default: 
                return true;
        }
    }

  private:
    /// The minimum MTU of all connected clients.
    std::atomic_uint_fast16_t min_mtu{23};
    /// Override the minimum MTU (0 means don't override, nonzero overrides if
    /// it's smaller than the minimum MTU of the clients).
    /// @see    @ref forceMinMTU()
    std::atomic_uint_fast16_t force_min_mtu{0};

    /// Find the smallest MTU of all clients. Used to compute the MIDI BLE 
    /// packet size.
    void updateMTU();

  public:
    /// Get the minimum MTU of all connected clients.
    uint16_t getMinMTU() const { return min_mtu; }

    /// Force the MTU to an artificially small value (used for testing).
    void forceMinMTU(uint16_t mtu);

  private:
    // BLE Server Callbacks
    void onConnect(BLEServer *pServer) override;
    void onDisconnect(BLEServer *pServer) override;

    // BLE Characteristic Callbacks
    void onRead(BLECharacteristic *pCharacteristic) override;
    void onWrite(BLECharacteristic *pCharacteristic) override;
};

END_CS_NAMESPACE