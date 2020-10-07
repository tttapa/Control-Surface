#pragma once

#include "BLEMIDI/BLEMIDI.hpp"
#include "BLEMIDI/BLEMIDI_Parser.hpp"
#include "BLEMIDI/BLEMIDIPacketBuilder.hpp"
#include "SerialMIDI_Interface.hpp"

#include <AH/Error/Error.hpp>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

#ifdef ESP32
#include <esp_pthread.h>
#endif

BEGIN_CS_NAMESPACE

struct ScopedThreadConfig {
#ifdef ESP32
#if 0 // New ESP-IDF
    ScopedThreadConfig(size_t stack_size, size_t priority, bool inherit_cfg, 
                       const char *thread_name, 
                       int pin_to_core = tskNO_AFFINITY) {
        esp_pthread_cfg_t cfg = esp_pthread_get_default_config();
        cfg.stack_size = stack_size;
        cfg.prio = priority;
        cfg.inherit_cfg = inherit_cfg;
        cfg.thread_name = thread_name;
        cfg.pin_to_core = pin_to_core;
        esp_pthread_set_cfg(&cfg);
    }

    ~ScopedThreadConfig() {
        esp_pthread_cfg_t cfg = esp_pthread_get_default_config();
        esp_pthread_set_cfg(&cfg);
    }
#else
    ScopedThreadConfig(size_t stack_size, size_t priority, bool inherit_cfg, 
                       const char *thread_name, 
                       int pin_to_core = tskNO_AFFINITY) {
        hadPreviousConfig = esp_pthread_get_cfg(&previousConfig) == ESP_OK;
        esp_pthread_cfg_t cfg;
        cfg.stack_size = stack_size;
        cfg.prio = priority;
        cfg.inherit_cfg = inherit_cfg;
        (void)thread_name;
        (void)pin_to_core;
        esp_pthread_set_cfg(&cfg);
    }

    ~ScopedThreadConfig() {
        if(hadPreviousConfig)
            esp_pthread_set_cfg(&previousConfig);
    }

    private:
      bool hadPreviousConfig;
      esp_pthread_cfg_t previousConfig;
#endif
#else
    ScopedThreadConfig(size_t stack_size, size_t priority, bool inherit_cfg, 
                       const char *thread_name, int pin_to_core = -1) {
        (void)stack_size;
        (void)priority;
        (void)inherit_cfg;
        (void)thread_name;
        (void)pin_to_core;
    }
#endif
};

/**
 * @brief   Bluetooth Low Energy MIDI Interface for the ESP32.
 * 
 * @ingroup MIDIInterfaces
 */
class BluetoothMIDI_Interface : public Parsing_MIDI_Interface,
                                public BLEServerCallbacks,
                                public BLECharacteristicCallbacks {

  private:
    // BLE Callbacks
    void onConnect(BLEServer *pServer) override {
        (void)pServer;
        DEBUGFN("Connected");
        updateMTU();
    };
    void onDisconnect(BLEServer *pServer) override {
        (void)pServer;
        DEBUGFN("Disonnected");
        updateMTU();
    }

    void onRead(BLECharacteristic *pCharacteristic) override {
        DEBUGFN("Read");
        // respond with no payload
        pCharacteristic->setValue(nullptr, 0);
    }
    void onWrite(BLECharacteristic *pCharacteristic) override {
        std::string value = pCharacteristic->getValue();
        const uint8_t *data =
            reinterpret_cast<const uint8_t *>(value.data());
        size_t len = value.size();
        DEBUGFN("Write [" << len << "] " << AH::HexDump(data, len));
        parse(data, len);
    }

  private:
    /// BLE connection manager.
    BLEMIDI bleMidi;
    /// MIDI Parser for incoming data.
    SerialMIDI_Parser parser;

    /// Builds outgoing MIDI BLE packets.
    BLEMIDIPacketBuilder packetbuilder;
    /// The minimum MTU of all connected clients.
    std::atomic_uint_fast16_t min_mtu{23};
    /// Override the minimum MTU (0 means don't override, nonzero overrides if
    /// it's smaller than the minimum MTU of the clients).
    /// @see    @ref forceMinMTU()
    std::atomic_uint_fast16_t force_min_mtu{0};

  private:
    /// Find the smallest MTU of all clients. Used to compute the MIDI BLE 
    /// packet size.
    void updateMTU() {
        uint16_t force_min_mtu_c = force_min_mtu;
        uint16_t ble_min_mtu = bleMidi.get_min_mtu();
        if (force_min_mtu_c == 0)
            min_mtu = ble_min_mtu;
        else 
            min_mtu = std::min(force_min_mtu_c, ble_min_mtu);
        DEBUGFN(NAMEDVALUE(min_mtu));
    }

  public:
    uint16_t getMinMTU() const {
        return min_mtu;
    }

    /// Force the MTU to an artificially small value (used for testing).
    void forceMinMTU(uint16_t mtu) {
        lock_t lock(mtx);
        force_min_mtu = mtu;
        updateMTU();
        if (packetbuilder.getSize() == 0)
            packetbuilder.setCapacity(min_mtu - 3);
    }

  private:
    // Synchronization for asynchronous BLE sending
    using lock_t = std::unique_lock<std::mutex>;
    std::mutex mtx;
    std::condition_variable cv;
    std::thread send_thread;
    std::atomic_bool stop_sending{false};

    volatile bool flushnow = false;

    std::chrono::milliseconds timeout{10};

  private:
    /// @param  lock 
    ///         Lock should be locked, will still be locked on exit.
    void flushImpl(lock_t &lock) {
        assert(lock.owns_lock());
        if (packetbuilder.empty())
            return;
        flushnow = true;
        lock.unlock();
        cv.notify_one();
        lock.lock();
        
        // Wait for flush to complete
        cv.wait(lock, [this]{ return !flushnow; });
        assert(lock.owns_lock());
    }

  public:
    void flush() {
        lock_t lock(mtx);
        flushImpl(lock);
    }

    // Called in a different task
    void handleSendEvents() {
        lock_t lock(mtx);
  
        // Wait for a packet to be started (or for a shutdown signal)
        cv.wait(lock, [this]{ return !packetbuilder.empty() || stop_sending; });
        // Wait for flush signal or timeout
        bool flushing = cv.wait_for(lock, timeout, [this]{ return flushnow; });

        // Send the packet over BLE, empty the buffer, and update the MTU
        bleMidi.notifyValue(packetbuilder.getBuffer(), packetbuilder.getSize());
        packetbuilder.reset();
        packetbuilder.setCapacity(min_mtu - 3);
            
        // Notify the main thread that the flush was done
        if (flushing) {
            flushnow = false;
            lock.unlock();
            cv.notify_one();
        }
    }

    void setTimeout(std::chrono::milliseconds timeout) {
        lock_t lock(mtx);
        this->timeout = timeout;
    }

  public:
    BluetoothMIDI_Interface() : Parsing_MIDI_Interface(parser) {}
    ~BluetoothMIDI_Interface() {
        stop_sending = true;
        lock_t lock(mtx);
        flushnow = true;
        lock.unlock();
        cv.notify_one();
        send_thread.join();
    }

    void begin() override {
        bleMidi.begin(this, this);
        // Need larger stack than default
        ScopedThreadConfig(4096, 3, true, "SendBLEMIDI");
        auto send = [this]{ while (!stop_sending) handleSendEvents(); };
        send_thread = std::thread(send);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    MIDIReadEvent read() override {
        update();                         // TODO
        return MIDIReadEvent::NO_MESSAGE; // TODO
    }

    void update() override {
        // TODO
    }

    void sendImpl(uint8_t header, uint8_t d1, uint8_t d2, uint8_t cn) override {
        (void)cn;
        lock_t lock(mtx);
        uint16_t timestamp = millis();
        if (!packetbuilder.add3B(header, d1, d2, timestamp)) {
            flushImpl(lock);
            packetbuilder.add3B(header, d1, d2, timestamp);
        }
        lock.unlock();
        cv.notify_one();
    }

    void sendImpl(uint8_t header, uint8_t d1, uint8_t cn) override {
        (void)cn;
        lock_t lock(mtx);
        uint16_t timestamp = millis();
        if (!packetbuilder.add2B(header, d1, timestamp)) {
            flushImpl(lock);
            packetbuilder.add2B(header, d1, timestamp);
        }
        lock.unlock();
        cv.notify_one();
    }

    void sendImpl(const uint8_t *data, size_t length, uint8_t cn) override {
        (void)cn;
        if (length < 2)
            return;

        lock_t lock(mtx);

        // Length of SysEx data without SysEx start/end
        length -= 2;
        // Data without SysEx start
        data += 1;
        // BLE MIDI timestamp
        uint16_t timestamp = millis();

        if (!packetbuilder.addSysEx(data, length, timestamp)) {
            flushImpl(lock);
            packetbuilder.addSysEx(data, length, timestamp);
        }
        while (data) {
            flushImpl(lock);
            packetbuilder.continueSysEx(data, length, timestamp);
        }
        lock.unlock();
        cv.notify_one();
    }

    void sendImpl(uint8_t rt, uint8_t cn) override {
        (void)cn;
        lock_t lock(mtx);
        uint16_t timestamp = millis();
        if (!packetbuilder.addRealTime(rt, timestamp)) {
            flushImpl(lock);
            packetbuilder.addRealTime(rt, timestamp);
        }
        lock.unlock();
        cv.notify_one();
    }

    void parse(const uint8_t *const data, const size_t len) {
        auto midiparser = [this](uint8_t databyte) { parse(databyte); };
        BLEMIDI_Parser(midiparser)(data, len);
    }

    void parse(uint8_t data) {
        event = parser.parse(data);
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
    }

    /// For tests.
    BLEMIDI &getBLEMIDI() { return bleMidi; }
};

END_CS_NAMESPACE