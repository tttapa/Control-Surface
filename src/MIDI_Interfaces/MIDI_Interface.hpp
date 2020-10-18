#pragma once

#include "MIDI_Pipes.hpp"
#include "MIDI_Staller.hpp"
#include "MIDI_Sender.hpp"
#include <AH/Containers/Updatable.hpp>
#include <Def/Def.hpp>
#include <Def/MIDIAddress.hpp>
#include <MIDI_Parsers/MIDI_Parser.hpp>

BEGIN_CS_NAMESPACE

constexpr auto MIDI_BAUD = 31250;

class MIDI_Callbacks;

/**
 * @brief   An abstract class for MIDI interfaces.
 */
class MIDI_Interface : public TrueMIDI_SinkSource,
                       public MIDI_Sender<MIDI_Interface>,
                       public AH::Updatable<MIDI_Interface>,
                       protected MIDIStaller {
  protected:
    MIDI_Interface() = default;
    MIDI_Interface(MIDI_Interface &&) = default;

  public:
    /// Destructor.
    virtual ~MIDI_Interface();

    /// Initialize the MIDI Interface.
    void begin() override {}
    /// Read the MIDI interface and call the callback if a message was received.
    void update() override = 0;

    /// @name   Default MIDI Interfaces
    /// @{

    /// Set this MIDI interface as the default interface.
    void setAsDefault();
    /// Return the default MIDI interface. If the default MIDI interface was
    /// configured explicitly using @ref setAsDefault(), that interface is 
    /// returned. If it wasn't set, or if that MIDI interface no longer exists,
    /// this function returns the newest MIDI interface, the one that was 
    /// constructed most recently. If no MIDI interfaces exist, `nullptr` is 
    /// returned.
    static MIDI_Interface *getDefault();

    /// @}

    /// @name   MIDI Input Callbacks
    /// @{

    /// Set the callbacks that will be called when a MIDI message is received.
    /// @param   cb
    ///          A pointer to the callback object.
    void setCallbacks(MIDI_Callbacks *cb) { this->callbacks = cb; }
    /// Set the callbacks that will be called when a MIDI message is received.
    /// @param   cb
    ///          A reference to the callback object.
    void setCallbacks(MIDI_Callbacks &cb) { setCallbacks(&cb); }

    /// @}

  protected:
    friend class MIDI_Sender<MIDI_Interface>;
    /// Low-level function for sending a MIDI channel message.
    virtual void sendChannelMessageImpl(ChannelMessage) = 0;
    /// Low-level function for sending a MIDI system common message.
    virtual void sendSysCommonImpl(SysCommonMessage) = 0;
    /// Low-level function for sending a system exclusive MIDI message.
    virtual void sendSysExImpl(SysExMessage) = 0;
    /// Low-level function for sending a MIDI real-time message.
    virtual void sendRealTimeImpl(RealTimeMessage) = 0;

  protected:
    /// Accept an incoming MIDI Channel message from the source pipe.
    void sinkMIDIfromPipe(ChannelMessage msg) override { send(msg); }
    /// Accept an incoming MIDI System Exclusive message from the source pipe.
    void sinkMIDIfromPipe(SysExMessage msg) override { send(msg); }
    /// Accept an incoming MIDI Real-Time message from the source pipe.
    void sinkMIDIfromPipe(RealTimeMessage msg) override { send(msg); }

  protected:
    /// Call the channel message callback and send the message to the sink pipe.
    bool onChannelMessage(ChannelMessage message);
    /// Call the SysEx message callback and send the message to the sink pipe.
    bool onSysExMessage(SysExMessage message);
    /// Call the real-time message callback and send the message to the sink 
    /// pipe.
    bool onRealTimeMessage(RealTimeMessage message);

  private:
    MIDI_Callbacks *callbacks = nullptr;

  private:
    static MIDI_Interface *DefaultMIDI_Interface;
};

END_CS_NAMESPACE
