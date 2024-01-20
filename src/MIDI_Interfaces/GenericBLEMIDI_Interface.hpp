#pragma once

#include <AH/Error/Error.hpp>

#include "BLEMIDI/BLEAPI.hpp"
#include "MIDI_Interface.hpp"

#include <chrono>

BEGIN_CS_NAMESPACE

/**
 * @brief   Bluetooth Low Energy MIDI Interface for the ESP32.
 * 
 * @ingroup MIDIInterfaces
 */
template <class BackendT>
class GenericBLEMIDI_Interface : public MIDI_Interface {
  public:
    using Backend = BackendT;
    Backend backend;

    template <class... Args>
    GenericBLEMIDI_Interface(Args &&...args)
        : backend {std::forward<Args>(args)...} {}

  public:
    /// Set the BLE device name. Must be called before @ref begin().
    void setName(const char *name);

    void begin() override;
    void end();
    bool isConnected() const { return backend.isConnected(); }

    MIDIReadEvent read();

    void update() override { MIDI_Interface::updateIncoming(this); }

    /// Set the timeout, the number of milliseconds to buffer the outgoing MIDI
    /// messages. A shorter timeout usually results in lower latency, but also
    /// causes more overhead, because more packets might be required.
    void setTimeout(std::chrono::milliseconds timeout) {
        backend.setTimeout(timeout);
    }

    BLESettings ble_settings;

  private:
#if !DISABLE_PIPES
    void handleStall() override { MIDI_Interface::handleStall(this); }
#endif

  public:
    /// Return the received channel voice message.
    ChannelMessage getChannelMessage() const;
    /// Return the received system common message.
    SysCommonMessage getSysCommonMessage() const;
    /// Return the received real-time message.
    RealTimeMessage getRealTimeMessage() const;
    /// Return the received system exclusive message.
    SysExMessage getSysExMessage() const;
    /// Get the BLE-MIDI timestamp of the latest MIDI message.
    /// @note Invalid for SysEx chunks (except the last chunk of a message).
    uint16_t getTimestamp() const;

  protected:
    // MIDI send implementations
    template <class L, class F>
    void sendImpl(L &lck, F add_to_buffer);
    void sendChannelMessageImpl(ChannelMessage) override;
    void sendSysCommonImpl(SysCommonMessage) override;
    void sendSysExImpl(SysExMessage) override;
    void sendRealTimeImpl(RealTimeMessage) override;
    void sendNowImpl() override;

  private:
    /// Incoming message that can be from retrieved using the
    /// `getChannelMessage()`, `getSysCommonMessage()`, `getRealTimeMessage()`
    /// and `getSysExMessage()` methods.
    typename Backend::IncomingMIDIMessage incomingMessage;
};

END_CS_NAMESPACE

#include "GenericBLEMIDI_Interface.ipp"
