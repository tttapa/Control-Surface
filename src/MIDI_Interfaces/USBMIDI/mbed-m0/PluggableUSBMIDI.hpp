#pragma once

#include <AH/Arduino-Wrapper.h>

#include <atomic>
#include <cstdint>
#include <tuple>

#include <USB/PluggableUSBDevice.h>
#include <drivers/Timeout.h>
#include <platform/Callback.h>

#include <AH/Settings/Warnings.hpp>
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

class PluggableUSBMIDI : protected arduino::internal::PluggableUSBModule {
  public:
    PluggableUSBMIDI();
    ~PluggableUSBMIDI();

  public:
    using setup_packet_t = USBDevice::setup_packet_t;
    using DeviceState = USBDevice::DeviceState;
    using microseconds = std::chrono::microseconds;

  public:
    /// Check if this class is connected and ready.
    bool connected() const;

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
    void init(EndpointResolver &resolver) override;
    void callback_state_change(DeviceState new_state) override;
    uint32_t callback_request(const setup_packet_t *setup,
                              USBDevice::RequestResult *result,
                              uint8_t **data) override;
    bool callback_request_xfer_done(const setup_packet_t *setup,
                                    bool aborted) override;
    bool callback_set_configuration(uint8_t configuration) override;
    void callback_set_interface(uint16_t interface, uint8_t alternate) override;

    const uint8_t *string_iinterface_desc() override;
    const uint8_t *configuration_desc(uint8_t index) override;
    uint8_t getProductVersion() override { return 16; }

  protected:
    std::atomic<bool> usb_connected{false};

  public:
    /// USB packet size. Must be a power of two.
    /// @todo   Why does increasing the packet size beyond 64 not work?
    static constexpr uint32_t PacketSize = 64;

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
        uint32_t available{0};
        uint32_t read_idx{0};
        uint32_t write_idx{0};
        bool reading{false};
    } reading;
    using rbuffer_t = Reading::Buffer;

    /// State for writing outgoing USB-MIDI data.
    struct Writing {
        struct Buffer {
            /// How many bytes are in the buffer.
            uint32_t size{0};
            /// Indicates that this buffer can be sent as soon as the previous
            /// one has been sent.
            /// This flag is changed from false to true when the buffer is
            /// scheduled to be sent, either because it is full or because the 
            /// timeout was triggered. If the flag is true, no further efforts
            /// should be made to send it. It is cleared in the USB ISR, after
            /// the buffer has actually been sent.
            bool ready_to_send{false};
            alignas(uint32_t) uint8_t buffer[PacketSize];
        } buffers[2];
        /// The index of the buffer that is currently being written to.
        uint32_t active_writebuffer{0};
        /// Buffer that is being sent.
        Buffer *sending{nullptr};
        /// Buffer to be sent in the timeout callback
        Buffer *send_timeout{nullptr};
        microseconds timeout_duration{1'000};
        microseconds error_timeout_duration{40'000};
        mbed::Timeout timeout;
        uint32_t errors{0};
    } writing;
    using wbuffer_t = Writing::Buffer;

    usb_ep_t bulk_in_ep;
    usb_ep_t bulk_out_ep;
    uint8_t config_descriptor[0x65];

    uint32_t write_impl(const uint32_t *msgs, uint32_t num_msgs,
                        bool nonblocking);
    std::tuple<uint32_t, wbuffer_t *, uint32_t> read_writebuf_size();
    void write_start_sync(uint8_t *buffer, uint32_t size);
    void send_in_callback(uint32_t activebuf_idx);
    bool send_now_impl_nonblock(uint32_t activebuf_idx);
    void timeout_callback();
    void in_callback();
    void out_callback();
};

END_CS_NAMESPACE
