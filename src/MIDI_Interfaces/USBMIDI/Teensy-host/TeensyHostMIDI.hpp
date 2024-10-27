#pragma once

#include <USBHost_t36.h>
#include <atomic>
#include <chrono>
#include <tuple>

#include <MIDI_Interfaces/USBMIDI/LowLevel/BulkRX.hpp>
#include <MIDI_Interfaces/USBMIDI/LowLevel/BulkTX.hpp>
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

/// Teensy USB Host MIDI driver
template <uint16_t MaxPacketSize = 512>
class TeensyHostMIDI
    : public USBDriver,
      protected BulkRX<TeensyHostMIDI<MaxPacketSize>, uint32_t, MaxPacketSize>,
      protected BulkTX<TeensyHostMIDI<MaxPacketSize>, uint32_t, MaxPacketSize> {
  public:
    TeensyHostMIDI(USBHost &) { init(); }

    using microseconds = std::chrono::microseconds;

    /// Try reading a 4-byte MIDI USB message.
    ///
    /// @return The message or 0x00000000 if no messages available.
    uint32_t read() {
        uint32_t packet = 0;
        read(packet);
        return packet;
    }

    using BulkRX<TeensyHostMIDI, uint32_t, MaxPacketSize>::read;
    using BulkTX<TeensyHostMIDI, uint32_t, MaxPacketSize>::write;
    using BulkTX<TeensyHostMIDI, uint32_t, MaxPacketSize>::write_nonblock;
    using BulkTX<TeensyHostMIDI, uint32_t, MaxPacketSize>::send_now;
    using BulkTX<TeensyHostMIDI, uint32_t, MaxPacketSize>::getWriteError;
    using BulkTX<TeensyHostMIDI, uint32_t, MaxPacketSize>::clearWriteError;

    /// Set the timeout, the number of microseconds to buffer the outgoing MIDI
    /// messages. A shorter timeout usually results in lower latency, but also
    /// causes more overhead, because more packets might be required.
    void setTimeout(microseconds timeout) { write_timeout_duration = timeout; }
    /// @todo   Actually implement this timeout
    void setErrorTimeout(microseconds timeout) {
        write_error_timeout_duration = timeout;
    }

    bool connectedForWrite() const { return txpipe; }

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
    Pipe_t *rxpipe, *txpipe;
    uint16_t rx_size, tx_size;
    uint8_t rx_ep, tx_ep;
    uint8_t rx_ep_type, tx_ep_type;
    Pipe_t mypipes[3] __attribute__((aligned(32)));
    Transfer_t mytransfers[7] __attribute__((aligned(32)));
    strbuf_t mystring_bufs[1];

  protected:
    friend class BulkTX<TeensyHostMIDI, uint32_t, MaxPacketSize>;
    void start_timeout();
    void cancel_timeout();
    void tx_start(const void *data, uint32_t size);
    void tx_start_timeout(const void *data, uint32_t size);
    void tx_start_isr(const void *data, uint32_t size);

    USBDriverTimer write_timeout {this};
    microseconds write_timeout_duration {1'000};
    microseconds write_error_timeout_duration {40'000};

  protected:
    friend class BulkRX<TeensyHostMIDI, uint32_t, MaxPacketSize>;
    void rx_start(void *data, uint32_t size);
    void rx_start_isr(void *data, uint32_t size);

  private:
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
