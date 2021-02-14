#pragma once

#include <USBHost_t36.h>
#include <atomic>
#include <chrono>
#include <tuple>

#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

/// Teensy USB Host MIDI driver
template <uint16_t MaxPacketSize = 512>
class TeensyHostMIDI : public USBDriver {
  public:
    TeensyHostMIDI(USBHost &host) { init(); }

    using microseconds = std::chrono::microseconds;

    /// Send a MIDI USB message. May block.
    ///
    /// @param  msg
    ///         The 4-byte MIDI USB message to send.
    void write(uint32_t msg);

    /// Send multiple MIDI USB messages. May block.
    ///
    /// @param  msgs
    ///         An array of 4-byte MIDI USB messages to send.
    /// @param  num_msgs
    ///         The number of messages in the array.
    void write(const uint32_t *msgs, uint32_t num_msgs);

    /// Send multiple MIDI USB messages. May block.
    template <size_t N>
    void write(const uint32_t (&msgs)[N]) {
        write(msgs, N);
    }

    /// Send multiple MIDI USB messages without blocking.
    ///
    /// @param  msgs
    ///         An array of 4-byte MIDI USB messages to send.
    /// @param  num_msgs
    ///         The number of messages in the array.
    /// @return The number of messages that were actually sent.
    uint32_t write_nonblock(const uint32_t *msgs, uint32_t num_msgs);

    /// Try reading a 4-byte MIDI USB message.
    ///
    /// @return The message or 0x00000000 if no messages available.
    uint32_t read();

    /// Try sending the buffered data now.
    /// Start transmitting the latest packet if possible, even if it isn't full
    /// yet. If the latest packet is empty, this function has no effect.
    void send_now();

    /// Set the timeout, the number of microseconds to buffer the outgoing MIDI
    /// messages. A shorter timeout usually results in lower latency, but also
    /// causes more overhead, because more packets might be required.
    void setTimeout(microseconds timeout) {
        writing.timeout_duration = timeout;
    }
    /// @todo   Actually implement this timeout
    void setErrorTimeout(microseconds timeout) {
        writing.error_timeout_duration = timeout;
    }

    /// Count how many USB packets were dropped.
    uint32_t getWriteError() const { return writing.errors; }
    /// Clear the counter of how many USB packets were dropped.
    void clearWriteError() { writing.errors = 0; }

  protected:
    bool claim(Device_t *device, int type, const uint8_t *descriptors,
               uint32_t len) override;
    void disconnect() override;

  protected:
    void timer_event(USBDriverTimer *whichtimer) override;

  protected:
    static void rx_callback(const Transfer_t *transfer);
    static void tx_callback(const Transfer_t *transfer);

  protected:
    void write_start(uint8_t *buffer, uint32_t size);
    void write_start_isr(uint8_t *buffer, uint32_t size);
    uint32_t write_finish(const Transfer_t *transfer);

    void read_start(uint8_t *buffer, uint32_t size);
    void read_start_isr(uint8_t *buffer, uint32_t size);
    uint32_t read_finish(const Transfer_t *transfer);

  protected:
    bool claim_if_midi(Device_t *device, int type, const uint8_t *descriptors,
                       uint32_t len);
    void init();

  private:
    Pipe_t *rxpipe;
    Pipe_t *txpipe;
    uint16_t rx_size;
    uint16_t tx_size;
    uint8_t rx_ep;
    uint8_t tx_ep;
    uint8_t rx_ep_type;
    uint8_t tx_ep_type;
    Pipe_t mypipes[3] __attribute__((aligned(32)));
    Transfer_t mytransfers[7] __attribute__((aligned(32)));
    strbuf_t mystring_bufs[1];

  public:
    /// USB packet size. Must be a power of two.
    static constexpr uint32_t PacketSize = MaxPacketSize;

  protected:
    static constexpr uint32_t SizeReserved = PacketSize + 1;
    static constexpr uint32_t NumRxPackets = 2;

    /// State for reading incoming USB-MIDI data.
    struct Reading {
        struct Buffer {
            uint32_t size = 0;
            uint32_t index = 0;
            alignas(uint32_t) uint8_t buffer[PacketSize];
        } buffers[NumRxPackets];
        std::atomic<uint32_t> available{0};
        std::atomic<uint32_t> read_idx{0};
        std::atomic<uint32_t> write_idx{0};
        std::atomic<bool> reading{false};
    } reading;
    using rbuffer_t = typename Reading::Buffer;

    /// State for writing outgoing USB-MIDI data.
    struct Writing {
        struct Buffer {
            std::atomic<uint32_t> size{0};
            std::atomic<bool> ready_to_send{false};
            alignas(uint32_t) uint8_t buffer[PacketSize];
        } buffers[2];
        std::atomic<uint32_t> active_writebuffer{0};
        std::atomic<Buffer *> sending{nullptr};
        std::atomic<Buffer *> send_timeout{nullptr};
        microseconds timeout_duration{1'000};
        microseconds error_timeout_duration{40'000};
        uint32_t errors{0};
    } writing;
    using wbuffer_t = typename Writing::Buffer;
    USBDriverTimer write_timeout{this};

  protected:
    uint32_t write_impl(const uint32_t *msgs, uint32_t num_msgs,
                        bool nonblocking);
    using writebuf_size_tup = std::tuple<uint32_t, wbuffer_t *, uint32_t>;
    writebuf_size_tup read_writebuf_size();
    bool send_now_impl_nonblock(uint32_t activebuf_idx);
    void timeout_callback();
    void in_callback(const Transfer_t *transfer);
    void out_callback(const Transfer_t *transfer);

  private:
    template <class T, size_t N>
    static constexpr size_t len(T (&)[N]) {
        return N;
    }
};

END_CS_NAMESPACE

#include "TeensyHostMIDI.ipp"
