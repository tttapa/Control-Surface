#pragma once

#include <AH/Error/Error.hpp>

#include "BLEMIDI/BLEAPI.hpp"
#include "MIDI_Interface.hpp"

#include <chrono>

BEGIN_CS_NAMESPACE

/**
 * @brief   Bluetooth Low Energy MIDI Interface.
 */
template <class Backend>
class GenericBLEMIDI_Interface : public MIDI_Interface {
  public:
    template <class... Args>
    GenericBLEMIDI_Interface(Args &&...args)
        : backend {std::forward<Args>(args)...} {}

  private:
    // MIDI send implementations
    template <class L, class F>
    void sendImpl(L &lck, F add_to_buffer);
    void sendChannelMessageImpl(ChannelMessage) override;
    void sendSysCommonImpl(SysCommonMessage) override;
    void sendSysExImpl(SysExMessage) override;
    void sendRealTimeImpl(RealTimeMessage) override;
    void sendNowImpl() override;

  private:
#if !DISABLE_PIPES
    void handleStall() override { MIDI_Interface::handleStall(this); }
#ifdef DEBUG_OUT
    const char *getName() const override { return "ble"; }
#endif
#endif

  public:
    /// @name   Initialization and polling
    /// @{

    /// Initialize the BLE hardware and start advertising as a MIDI over BLE
    /// peripheral.
    void begin() override;
    /// @todo   Currently not implemented.
    void end();
    /// Poll the backend (if necessary) and invoke the callbacks for any
    /// received MIDI messages, as well as sending them over the pipes connected
    /// to this interface.
    void update() override { MIDI_Interface::updateIncoming(this); }
    /// Returns true if a BLE central is currently connected.
    /// @note   This is useful for e.g. turning on/off an LED or showing a
    ///         message to the user, but it's not all that useful for anything
    ///         else, because the connection could be terminated at any moment.
    bool isConnected() const { return backend.isConnected(); }

    /// @}

  public:
    /// @name   Reading incoming MIDI messages
    /// @{

    /// Try reading and parsing a single incoming MIDI message.
    /// @return Returns the type of the message read, or
    ///         `MIDIReadEvent::NO_MESSAGE` if no MIDI message was available.
    MIDIReadEvent read();

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

    /// @}

  private:
    /// Incoming message that can be from retrieved using the
    /// `getChannelMessage()`, `getSysCommonMessage()`, `getRealTimeMessage()`
    /// and `getSysExMessage()` methods.
    typename Backend::IncomingMIDIMessage incomingMessage;

  public:
    /// @name   BLE configuration options
    /// @{

    /// Set the BLE device name. Must be called before @ref begin().
    /// Length is limited by the size of the BLE advertising packets.
    void setName(const char *name);
    /// Set the timeout, the number of milliseconds to buffer the outgoing MIDI
    /// messages. A shorter timeout usually results in lower latency, but also
    /// causes more overhead, because more packets might be required.
    void setTimeout(std::chrono::milliseconds timeout) {
        backend.setTimeout(timeout);
    }
    /// BLE backend configuration option.
    BLESettings ble_settings;

    /// @}

  public:
    /// Low-level BLE backend for sending and receiving MIDI over BLE packets.
    Backend backend;
};

END_CS_NAMESPACE

#include "GenericBLEMIDI_Interface.ipp"
