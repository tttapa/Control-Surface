#pragma once

#include <cstddef>

#include <MIDI_Interfaces/USBMIDI/util/Atomic.hpp>
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

/// Sends Bulk packets (IN for device mode, OUT for host mode)
template <class Derived, class MessageTypeT, uint16_t MaxPacketSizeV>
struct BulkTX {
  public:
    using MessageType = MessageTypeT;

    /// Send a MIDI USB message. May block.
    ///
    /// @param  msg
    ///         The 4-byte MIDI USB message to send.
    void write(MessageType msg);

    /// Send multiple MIDI USB messages. May block.
    ///
    /// @param  msgs
    ///         An array of 4-byte MIDI USB messages to send.
    /// @param  num_msgs
    ///         The number of messages in the array.
    void write(const MessageType *msgs, uint32_t num_msgs);

    /// Send multiple MIDI USB messages. May block.
    template <size_t N>
    void write(const MessageType (&msgs)[N]) {
        write(msgs, N);
    }

    /// Send multiple MIDI USB messages without blocking.
    ///
    /// @param  msgs
    ///         An array of 4-byte MIDI USB messages to send.
    /// @param  num_msgs
    ///         The number of messages in the array.
    /// @return The number of messages that were actually sent.
    uint32_t write_nonblock(const MessageType *msgs, uint32_t num_msgs);

    /// Try sending the buffered data now.
    /// Start transmitting the latest packet if possible, even if it isn't full
    /// yet. If the latest packet is empty, this function has no effect.
    void send_now();

    /// Check if all transfers have completed.
    bool is_done() const;

  protected:
    void reset(uint16_t packet_size = MaxPacketSize);

  private:
    static constexpr uint16_t MaxPacketSize = MaxPacketSizeV;
    static constexpr uint16_t SizeReserved = MaxPacketSize + 1;

  protected:
    // Derived should implement the following methods:

    /// Start a timeout (e.g. using a timer interrupt) that later calls
    /// @ref timeout_callback().
    void start_timeout() = delete;
    /// Cancel the timeout started by @ref timeout_callback().
    void cancel_timeout() = delete;
    /// Start a USB transfer (from the main program).
    void tx_start(const void *data, uint32_t size) = delete;
    /// Start a USB transfer (from the timeout callback).
    void tx_start_timeout(const void *data, uint32_t size) = delete;
    /// Start a USB transfer (from the USB interrupt handler).
    void tx_start_isr(const void *data, uint32_t size) = delete;

  private:
    constexpr static std::memory_order mo_seq = std::memory_order_seq_cst;
    constexpr static std::memory_order mo_rel = std::memory_order_release;
    constexpr static std::memory_order mo_acq = std::memory_order_acquire;
    constexpr static std::memory_order mo_rlx = std::memory_order_relaxed;
    constexpr static std::memory_order mo_acq_rel = std::memory_order_acq_rel;

    /// State for writing outgoing USB-MIDI data.
    struct Writing {
        struct Buffer {
            uint16_t size {0};
            alignas(MessageType) uint8_t buffer[MaxPacketSize];
        } buffers[2];
        interrupt_atomic<Buffer *> active_writebuffer {&buffers[0]};
        interrupt_atomic<Buffer *> sending {nullptr};
        interrupt_atomic<Buffer *> send_later {nullptr};
        interrupt_atomic<Buffer *> send_now {nullptr};
        uint16_t packet_size = MaxPacketSize;
    } writing;
    using wbuffer_t = typename Writing::Buffer;

    uint32_t index_of(wbuffer_t *p) const { return p - writing.buffers; }
    wbuffer_t *other_buf(wbuffer_t *p) {
        return &writing.buffers[!index_of(p)];
    }
    uint32_t write_impl(const MessageType *msgs, uint32_t num_msgs);

  protected:
    void timeout_callback();
    void tx_callback();
};

END_CS_NAMESPACE

#include "BulkTX.ipp"
