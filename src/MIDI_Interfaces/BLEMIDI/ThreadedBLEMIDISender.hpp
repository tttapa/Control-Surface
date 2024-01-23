#pragma once

#include <Settings/NamespaceSettings.hpp>

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "BLEAPI.hpp"
#include <MIDI_Interfaces/BLEMIDI/BLEMIDIPacketBuilder.hpp>

BEGIN_CS_NAMESPACE

/// Class that manages a background thread that sends BLE packets asynchronously.
template <class Derived>
class ThreadedBLEMIDISender {
  public:
    ThreadedBLEMIDISender() = default;
    ThreadedBLEMIDISender(const ThreadedBLEMIDISender &) = delete;
    ThreadedBLEMIDISender &operator=(const ThreadedBLEMIDISender &) = delete;
    ~ThreadedBLEMIDISender();

    /// Start the background thread.
    void begin();

    struct ProtectedBuilder;

    /// Acquire exclusive access to the buffer to be sent by the timer.
    /// @return A RAII wrapper that automatically releases the buffer upon
    ///         destruction. Just make sure you don't keep any pointers to the
    ///         `packet` member.
    ProtectedBuilder acquirePacket();
    /// Release exclusive access to the buffer and notify the sender thread that
    /// data is available.
    void releasePacketAndNotify(ProtectedBuilder &lck);

    /// Sends the data immediately without waiting for the timeout.
    void sendNow(ProtectedBuilder &lck);

    /// Set the maximum transmission unit of the Bluetooth link. Used to compute
    /// the MIDI BLE packet size.
    void updateMTU(uint16_t mtu);
    /// Get the minimum MTU of all connected clients.
    uint16_t getMinMTU() const { return min_mtu; }
    /// Force the MTU to an artificially small value (used for testing).
    void forceMinMTU(uint16_t mtu);

    /// Set the timeout, the number of milliseconds to buffer the outgoing MIDI
    /// messages.
    void setTimeout(std::chrono::milliseconds timeout);

  private:
    /// Actually perform the BLE notification with the given data.
    void sendData(BLEDataView) = delete; // should be implemented by subclass

    /// Function that waits for BLE packets and sends them in the background.
    /// It either sends them after a timeout (a given number of milliseconds
    /// after the first data was added to the packet), or immediately when it
    /// receives a flush signal from the main thread.
    bool handleSendEvents();

  private:
    struct {
        /// View of the data to send
        BLEMIDIPacketBuilder packet;
        /// Flag to stop the background thread.
        bool stop = false;
        /// Flag to tell the sender thread to send the packet immediately.
        bool flush = false;
        /// Timeout before the sender thread sends a packet.
        /// @see @ref setTimeout()
        std::chrono::milliseconds timeout {10};
        /// Lock to protect all shared data in this struct.
        std::mutex mtx;
    } shared {};
    /// Condition variable used by the background sender thread to wait for
    /// data to send, and for the main thread to wait for the data to be flushed
    /// by the sender thread.
    std::condition_variable cv;
    /// Lock type used to lock the mutex
    using lock_t = std::unique_lock<std::mutex>;
    /// The background thread responsible for sending the data.
    std::thread send_thread;

  private:
    /// The minimum MTU of all connected clients.
    std::atomic_uint_fast16_t min_mtu {23};
    /// Override the minimum MTU (0 means don't override, nonzero overrides if
    /// it's smaller than the minimum MTU of the clients).
    /// @see    @ref forceMinMTU()
    std::atomic_uint_fast16_t force_min_mtu {515};

  public:
    struct ProtectedBuilder {
        BLEMIDIPacketBuilder *packet;
        lock_t lck;
    };
};

END_CS_NAMESPACE

#include "ThreadedBLEMIDISender.ipp"
