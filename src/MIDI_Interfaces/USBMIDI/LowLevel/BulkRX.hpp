#pragma once

#include <cstdint>

#include <MIDI_Interfaces/USBMIDI/util/Atomic.hpp>
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

/// Receives Bulk packets (OUT for device mode, IN for host mode)
template <class Derived, class MessageTypeT, uint16_t MaxPacketSizeV>
struct BulkRX {
  public:
    using MessageType = MessageTypeT;

    /// Try reading a single message.
    /// @return Whether a message was available.
    bool read(MessageType &message);

  protected:
    void reset(uint16_t packet_size = MaxPacketSize);

  private:
    static constexpr uint16_t MaxPacketSize = MaxPacketSizeV;
    static constexpr uint16_t SizeReserved = MaxPacketSize + 1;
    static constexpr uint16_t NumRxPackets = MaxPacketSize == 64 ? 16 : 4;

  protected:
    // Derived should implement the following methods:

    /// Start a USB transfer (from the main program).
    void rx_start(void *data, uint32_t size) = delete;
    /// Start a USB transfer (from the USB interrupt handler).
    void rx_start_isr(void *data, uint32_t size) = delete;

  private:
    constexpr static std::memory_order mo_seq = std::memory_order_seq_cst;
    constexpr static std::memory_order mo_rel = std::memory_order_release;
    constexpr static std::memory_order mo_acq = std::memory_order_acquire;
    constexpr static std::memory_order mo_rlx = std::memory_order_relaxed;
    constexpr static std::memory_order mo_acq_rel = std::memory_order_acq_rel;

    /// State for reading incoming USB data.
    struct Reading {
        struct Buffer {
            uint16_t size = 0;
            uint16_t index = 0;
            alignas(uint32_t) uint8_t buffer[MaxPacketSize];
        } buffers[NumRxPackets];
        interrupt_atomic<uint32_t> available {0};
        uint32_t read_idx {0};
        interrupt_atomic<uint32_t> write_idx {0};
        interrupt_atomic<bool> reading {false};
        uint16_t packet_size = MaxPacketSize;
    } reading;
    using rbuffer_t = std::remove_reference_t<decltype(reading.buffers[0])>;

  protected:
    void rx_callback(uint32_t num_bytes_read);
};

END_CS_NAMESPACE

#include "BulkRX.ipp"
