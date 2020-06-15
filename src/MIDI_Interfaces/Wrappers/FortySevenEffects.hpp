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
        this->midimsg.CN = 0;
    }

    /// Get the latest system exclusive message from the given MIDI interface.
    template <class MidiInterface>
    void updateSysExMessage(const MidiInterface &interface) {
        this->sysex.data = interface.getSysExArray();
        this->sysex.length = interface.getSysExArrayLength();
        this->sysex.CN = 0;
    }

    /// Get the latest real-time message from the given MIDI interface.
    template <class MidiInterface>
    void updateRealTimeMessage(const MidiInterface &interface) {
        this->rtmsg.message = interface.getType();
        this->rtmsg.CN = 0;
    }

    /// Temporarily saves a pointer to the MIDI parser's SysEx buffer.
    SysExMessage sysex = {nullptr, 0, 0};

  public:
    SysExMessage getSysExMessage() const override { return sysex; }
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
class FortySevenEffectsMIDI_Interface : public Parsing_MIDI_Interface {
  public:
    FortySevenEffectsMIDI_Interface(MidiInterface &&midi)
        : Parsing_MIDI_Interface(parser),
          midi(std::forward<MidiInterface>(midi)) {}

    void begin() override { midi.begin(MIDI_CHANNEL_OMNI); }

    MIDIReadEvent read() override {
        if (!midi.read())      // Update the MIDI input and check if there's
            return MIDIReadEvent::NO_MESSAGE; // a new message available
        auto type = midi.getType();
        if (midi.isChannelMessage(type)) { // Channel
            parser.updateChannelMessage(midi);
            return MIDIReadEvent::CHANNEL_MESSAGE;
        }
        if (type == uint8_t(MIDIMessageType::SYSEX_START)) { // SysEx
            parser.updateSysExMessage(midi);
            return MIDIReadEvent::SYSEX_MESSAGE;
        }
        if (type >= uint8_t(MIDIMessageType::TIMING_CLOCK)) { // Real-Time
            parser.updateRealTimeMessage(midi);
            return MIDIReadEvent::REALTIME_MESSAGE;
        }
        return MIDIReadEvent::NO_MESSAGE;
    }

  protected:
    void sendImpl(uint8_t header, uint8_t d1, uint8_t d2, uint8_t cn) override {
        uint8_t m = header & 0xF0;
        uint8_t c = header & 0x0F;
        // channel is zero-based in Control Surface, one-based in MIDI 47 Fx
        midi.send(static_cast<MIDI_NAMESPACE::MidiType>(m), d1, d2, c + 1);
        (void)cn;
    }
    void sendImpl(uint8_t header, uint8_t d1, uint8_t cn) override {
        uint8_t m = header & 0xF0;
        uint8_t c = header & 0x0F;
        // channel is zero-based in Control Surface, one-based in MIDI 47 Fx
        midi.send(static_cast<MIDI_NAMESPACE::MidiType>(m), d1, 0, c + 1);
        // MIDI 47 Fx checks message type to handle 2-byte messages separately
        (void)cn;
    }
    void sendImpl(const uint8_t *data, size_t length, uint8_t cn) {
        midi.sendSysEx(length, data, true);
        // true indicates that the array contains the SysEx start and stop bytes
        (void)cn;
    }
    void sendImpl(uint8_t rt, uint8_t cn) override {
        midi.sendRealTime(static_cast<MIDI_NAMESPACE::MidiType>(rt));
        (void)cn;
    }

  private:
    FortySevenEffectsMIDI_Parser parser;
    MidiInterface midi;
};

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()