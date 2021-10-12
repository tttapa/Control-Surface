#pragma once

#include <AH/Error/Error.hpp>
#include <MIDI_Interfaces/MIDI_Interface.hpp>

AH_DIAGNOSTIC_WERROR()

AH_DIAGNOSTIC_EXTERNAL_HEADER()
#include <MIDI.h>
AH_DIAGNOSTIC_POP()

BEGIN_CS_NAMESPACE

/**
 * @brief   Wrapper class for the FortySevenEffects MIDI parser.
 * 
 * @see @ref FortySevenEffectsMIDI_Interface
 */
class FortySevenEffectsMIDI_Parser : public MIDI_Parser {
  private:
    template <class MidiInterface>
    friend class FortySevenEffectsMIDI_Interface;

    /// Get the latest channel message from the given MIDI interface.
    template <class MidiInterface>
    void updateChannelMessage(const MidiInterface &interface) {
        uint8_t channel = interface.getChannel() - 1;
        this->midimsg.header = interface.getType() | channel;
        this->midimsg.data1 = interface.getData1();
        this->midimsg.data2 = interface.getData2();
        this->midimsg.cable = CABLE_1;
    }

    /// Get the latest system exclusive message from the given MIDI interface.
    template <class MidiInterface>
    void updateSysExMessage(const MidiInterface &interface) {
        this->sysex.data = interface.getSysExArray();
        this->sysex.length = interface.getSysExArrayLength();
        this->sysex.cable = CABLE_1;
    }

    /// Get the latest system common message from the given MIDI interface.
    template <class MidiInterface>
    void updateSysCommonMessage(const MidiInterface &interface) {
        this->midimsg.header = interface.getType();
        this->midimsg.data1 = interface.getData1();
        this->midimsg.data2 = interface.getData2();
        this->midimsg.cable = CABLE_1;
    }

    /// Get the latest real-time message from the given MIDI interface.
    template <class MidiInterface>
    void updateRealTimeMessage(const MidiInterface &interface) {
        this->rtmsg.message = interface.getType();
        this->rtmsg.cable = CABLE_1;
    }

    /// Temporarily saves a pointer to the MIDI parser's SysEx buffer.
    SysExMessage sysex = {nullptr, 0, CABLE_1};

  public:
    /// Get the latest SysEx message.
    SysExMessage getSysExMessage() const { return sysex; }
};

/**
 * @brief   Class that wraps the FortySevenEffects MIDI library. 
 * 
 * This wrapper allows you to use MIDI interfaces that inherit from the 
 * FortySevenEffects MIDI library as a native Control Surface MIDI Interface.
 * 
 * It's not recommended to use this wrapper for normal Serial or USB MIDI 
 * interfaces, you should use the @ref HardwareSerialMIDI_Interface and 
 * @ref USBMIDI_Interface classes for that (see @ref MIDIInterfaces).
 * 
 * It can be useful, however, to use the AppleMIDI library, which uses the 
 * FortySevenEffects MidiInterface as its main public API. 
 * 
 * @tparam  MidiInterface
 *          The type of FortySevenEffects MIDI interface to use. You should
 *          probably just use `decltype` as demonstrated in the 
 *          @ref AppleMIDI.ino example.
 * 
 * @ingroup MIDIInterfaces
 */
template <class MidiInterface>
class FortySevenEffectsMIDI_Interface : public MIDI_Interface {
  public:
    FortySevenEffectsMIDI_Interface(MidiInterface &&midi)
        : midi(std::forward<MidiInterface>(midi)) {}

    void begin() override { midi.begin(MIDI_CHANNEL_OMNI); }

    MIDIReadEvent read() {
        if (!midi.read()) // Update the MIDI input and check if there's
            return MIDIReadEvent::NO_MESSAGE; // a new message available
        auto type = midi.getType();
        if (type <= uint8_t(MIDIMessageType::PITCH_BEND)) { // Channel
            parser.updateChannelMessage(midi);
            return MIDIReadEvent::CHANNEL_MESSAGE;
        } else if (type == uint8_t(MIDIMessageType::SYSEX_START)) { // SysEx
            parser.updateSysExMessage(midi);
            return MIDIReadEvent::SYSEX_MESSAGE;
        } else if (type <= uint8_t(MIDIMessageType::TUNE_REQUEST)) { // SysComm
            parser.updateSysCommonMessage(midi);
            return MIDIReadEvent::SYSCOMMON_MESSAGE;
        } else if (type == uint8_t(MIDIMessageType::SYSEX_END)) { // SysEx
            // ignore
        } else { // Real-Time
            parser.updateRealTimeMessage(midi);
            return MIDIReadEvent::REALTIME_MESSAGE;
        }
        return MIDIReadEvent::NO_MESSAGE;
    }

    /// Return the received channel voice message.
    ChannelMessage getChannelMessage() const {
        return parser.getChannelMessage();
    }
    /// Return the received real-time message.
    RealTimeMessage getRealTimeMessage() const {
        return parser.getRealTimeMessage();
    }
    /// Return the received system common message.
    SysCommonMessage getSysCommonMessage() const {
        return parser.getSysCommonMessage();
    }
    /// Return the received system exclusive message.
    SysExMessage getSysExMessage() const { return parser.getSysExMessage(); }

    void update() override {
        MIDIReadEvent event = read();
        while (event != MIDIReadEvent::NO_MESSAGE) { // As long as there are
                                                     // incoming messages
            dispatchMIDIEvent(event);
            event = read();
        }
        // TODO: check if we should block the pipe
    }

  protected:
    void dispatchMIDIEvent(MIDIReadEvent event) {
        switch (event) {
            case MIDIReadEvent::NO_MESSAGE: break;
            case MIDIReadEvent::CHANNEL_MESSAGE:
                onChannelMessage(getChannelMessage());
                break;
            case MIDIReadEvent::SYSEX_MESSAGE:
                onSysExMessage(getSysExMessage());
                break;
            case MIDIReadEvent::REALTIME_MESSAGE:
                onRealTimeMessage(getRealTimeMessage());
                break;
            case MIDIReadEvent::SYSCOMMON_MESSAGE:
                onSysCommonMessage(getSysCommonMessage());
                break;
            default: break;
        }
    }

  protected:
    void sendChannelMessageImpl(ChannelMessage msg) override {
        midi.send(static_cast<MIDI_NAMESPACE::MidiType>(msg.getMessageType()),
                  msg.getData1(), msg.getData2(),
                  msg.getChannel().getOneBased());
        // channel is zero-based in Control Surface, one-based in MIDI 47 Fx
    }
    void sendSysCommonImpl(SysCommonMessage msg) override {
        midi.sendCommon(
            static_cast<MIDI_NAMESPACE::MidiType>(msg.getMessageType()),
            msg.getData14bit());
    }
    void sendSysExImpl(SysExMessage msg) {
        midi.sendSysEx(msg.length, msg.data, true);
        // true indicates that the array contains the SysEx start and stop bytes
    }
    void sendRealTimeImpl(RealTimeMessage msg) override {
        midi.sendRealTime(static_cast<MIDI_NAMESPACE::MidiType>(msg.message));
    }
    void sendNowImpl() override { /* TODO */
    }

  private:
    void handleStall() override {
        auto stallername = MIDIStaller::getNameNull(getStaller());
        ERROR(F("Not implemented (stalled by ") << stallername << ')', 0x1349);
        (void)stallername;
    }

  private:
    FortySevenEffectsMIDI_Parser parser;
    MidiInterface midi;
};

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()