#pragma once

#include <AH/Arduino-Wrapper.h>

#include <cstdint>
#include <utility>

#include <USB/PluggableUSBDevice.h>
#include <drivers/Timeout.h>
#include <platform/Callback.h>

#include <MIDI_Interfaces/USBMIDI/LowLevel/BulkRX.hpp>
#include <MIDI_Interfaces/USBMIDI/LowLevel/BulkTX.hpp>
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

class PluggableUSBMIDI : protected arduino::internal::PluggableUSBModule,
                         protected BulkRX<PluggableUSBMIDI, uint32_t, 64>,
                         protected BulkTX<PluggableUSBMIDI, uint32_t, 64> {
  public:
    PluggableUSBMIDI();
    ~PluggableUSBMIDI();

  public:
    using setup_packet_t = USBDevice::setup_packet_t;
    using DeviceState = USBDevice::DeviceState;
    using microseconds = std::chrono::microseconds;

  private:
    // BulkTX/RX API
    friend class BulkTX<PluggableUSBMIDI, uint32_t, 64>;
    friend class BulkRX<PluggableUSBMIDI, uint32_t, 64>;
    static constexpr uint32_t get_packet_size() { return 64; }

    void start_timeout() {
        auto cb = mbed::callback(this, &PluggableUSBMIDI::timeout_callback);
        timeout.attach(std::move(cb), timeout_duration);
    }
    void cancel_timeout() { timeout.detach(); }
    void tx_start(const void *data, uint32_t size) {
        auto data_u8 = reinterpret_cast<uint8_t *>(const_cast<void *>(data));
        write_start(bulk_in_ep, data_u8, size);
    }
    void tx_start_timeout(const void *data, uint32_t size) {
        tx_start(data, size);
    }
    void tx_start_isr(const void *data, uint32_t size) { tx_start(data, size); }
    void rx_start(void *data, uint32_t size) {
        read_start(bulk_out_ep, reinterpret_cast<uint8_t *>(data), size);
    }
    void rx_start_isr(void *data, uint32_t size) { rx_start(data, size); }

  public:
    /// Check if this class is connected and ready.
    bool connected() const;

    /// Try reading a 4-byte MIDI USB message.
    ///
    /// @return The message or 0x00000000 if no messages available.
    uint32_t read() {
        uint32_t data = 0;
        read(data);
        return data;
    }

    using BulkRX<PluggableUSBMIDI, uint32_t, 64>::read;
    using BulkTX<PluggableUSBMIDI, uint32_t, 64>::write;
    using BulkTX<PluggableUSBMIDI, uint32_t, 64>::write_nonblock;
    using BulkTX<PluggableUSBMIDI, uint32_t, 64>::send_now;

    /// Set the timeout, the number of microseconds to buffer the outgoing MIDI
    /// messages. A shorter timeout usually results in lower latency, but also
    /// causes more overhead, because more packets might be required.
    void setTimeout(microseconds timeout) { timeout_duration = timeout; }
    /// @todo   Actually implement this timeout
    void setErrorTimeout(microseconds timeout) {
        error_timeout_duration = timeout;
    }

    /// Count how many USB packets were dropped.
    uint32_t getWriteError() const { return write_errors; }
    /// Clear the counter of how many USB packets were dropped.
    uint32_t clearWriteError() { return std::exchange(write_errors, 0); }

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
    interrupt_atomic<bool> usb_connected {false};
    microseconds timeout_duration {1'000};
    microseconds error_timeout_duration {40'000};
    mbed::Timeout timeout;
    uint32_t write_errors {0};

    usb_ep_t bulk_in_ep;
    usb_ep_t bulk_out_ep;
    uint8_t config_descriptor[0x65];

    // Interrupt handlers
    void timeout_callback() {
        BulkTX<PluggableUSBMIDI, uint32_t, 64>::timeout_callback();
    }
    void in_callback() {
        assert_locked();
        write_finish(bulk_in_ep);
        BulkTX<PluggableUSBMIDI, uint32_t, 64>::tx_callback();
    }
    void out_callback() {
        assert_locked();
        uint32_t num_bytes_read = read_finish(bulk_out_ep);
        BulkRX::rx_callback(num_bytes_read);
    }
};

END_CS_NAMESPACE
