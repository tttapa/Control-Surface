#pragma once

#include <Settings/NamespaceSettings.hpp>

#include <chrono>

#include "BLEAPI.hpp"
#include <MIDI_Interfaces/BLEMIDI/BLEMIDIPacketBuilder.hpp>

BEGIN_CS_NAMESPACE

/// Class that buffers MIDI BLE packets.
template <class Derived>
class PollingBLEMIDISender {
  public:
    PollingBLEMIDISender() = default;
    PollingBLEMIDISender(const PollingBLEMIDISender &) = delete;
    PollingBLEMIDISender &operator=(const PollingBLEMIDISender &) = delete;
    ~PollingBLEMIDISender();

    /// Initialize.
    void begin();

    /// RAII lock for access to the packet builder.
    struct ProtectedBuilder;
    /// Acquire exclusive access to the buffer.
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

  private:
    /// View of the data to send
    BLEMIDIPacketBuilder packet;
    /// Timeout before the sender thread sends a packet.
    /// @see @ref setTimeout()
    unsigned long timeout {10};
    /// Time point when the packet was started.
    unsigned long packet_start_time {0};

  private:
    /// The minimum MTU of all connected clients.
    uint16_t min_mtu {23};
    /// Override the minimum MTU (0 means don't override, nonzero overrides if
    /// it's smaller than the minimum MTU of the clients).
    /// @see    @ref forceMinMTU()
    uint16_t force_min_mtu {515};

  public:
    struct ProtectedBuilder {
        BLEMIDIPacketBuilder *packet;
    };
};

END_CS_NAMESPACE

#include "PollingBLEMIDISender.ipp"
