#pragma once

#include <AH/Containers/CRTP.hpp>
#include <MIDI_Parsers/MIDI_MessageTypes.hpp>

BEGIN_CS_NAMESPACE

class MIDI_Interface;

// LCOV_EXCL_START

/**
 * @brief   A class for callbacks from MIDI input.
 */
class MIDI_Callbacks {
  public:
    /// Callback for incoming MIDI Channel Messages (notes, control change,
    /// pitch bend, etc.)
    virtual void onChannelMessage(MIDI_Interface &, ChannelMessage) {}
    /// Callback for incoming MIDI System Exclusive Messages.
    virtual void onSysExMessage(MIDI_Interface &, SysExMessage) {}
    /// Callback for incoming MIDI System Common Messages.
    virtual void onSysCommonMessage(MIDI_Interface &, SysCommonMessage) {}
    /// Callback for incoming MIDI Real-Time Messages.
    virtual void onRealTimeMessage(MIDI_Interface &, RealTimeMessage) {}

    /// Destructor.
    virtual ~MIDI_Callbacks() = default;
};

// LCOV_EXCL_STOP

template <class Derived>
class FineGrainedMIDI_Callbacks : public MIDI_Callbacks {
  protected:
    /// @anchor FineGrainedMIDI_Callbacks_MIDI_Callback_Functions
    /// @name MIDI Callback Functions
    /// @{
    // clang-format off
    void onNoteOff(Channel channel, uint8_t note, uint8_t velocity, Cable cable);
    void onNoteOn(Channel channel, uint8_t note, uint8_t velocity, Cable cable);
    void onKeyPressure(Channel channel, uint8_t note, uint8_t pressure, Cable cable);
    void onControlChange(Channel channel, uint8_t controller, uint8_t value, Cable cable);
    void onProgramChange(Channel channel, uint8_t program, Cable cable);
    void onChannelPressure(Channel channel, uint8_t pressure, Cable cable);
    void onPitchBend(Channel channel, uint16_t bend, Cable cable);
    void onSystemExclusive(SysExMessage message);
    void onTimeCodeQuarterFrame(uint8_t data, Cable cable);
    void onSongPosition(uint16_t beats, Cable cable);
    void onSongSelect(uint8_t songnumber, Cable cable);
    void onTuneRequest(Cable cable);
    void onClock(Cable cable);
    void onStart(Cable cable);
    void onContinue(Cable cable);
    void onStop(Cable cable);
    void onActiveSensing(Cable cable);
    void onSystemReset(Cable cable);
    // clang-format on
    /// @}

    void onChannelMessage(MIDI_Interface &, ChannelMessage msg) override {
        using MMT = MIDIMessageType;
        switch (msg.getMessageType()) {
            case MMT::None: break;
            case MMT::NoteOff:
                CRTP(Derived).onNoteOff(msg.getChannel(), msg.getData1(),
                                        msg.getData2(), msg.getCable());
                break;
            case MMT::NoteOn:
                CRTP(Derived).onNoteOn(msg.getChannel(), msg.getData1(),
                                       msg.getData2(), msg.getCable());
                break;
            case MMT::KeyPressure:
                CRTP(Derived).onKeyPressure(msg.getChannel(), msg.getData1(),
                                            msg.getData2(), msg.getCable());
                break;
            case MMT::ControlChange:
                CRTP(Derived).onControlChange(msg.getChannel(), msg.getData1(),
                                              msg.getData2(), msg.getCable());
                break;
            case MMT::ProgramChange:
                CRTP(Derived).onProgramChange(msg.getChannel(), msg.getData1(),
                                              msg.getCable());
                break;
            case MMT::ChannelPressure:
                CRTP(Derived).onChannelPressure(msg.getChannel(),
                                                msg.getData1(), msg.getCable());
                break;
            case MMT::PitchBend:
                CRTP(Derived).onPitchBend(msg.getChannel(), msg.getData14bit(),
                                          msg.getCable());
                break;
            case MMT::SysExStart:
            case MMT::MTCQuarterFrame:
            case MMT::SongPositionPointer:
            case MMT::SongSelect:
            case MMT::UndefinedSysCommon1:
            case MMT::UndefinedSysCommon2:
            case MMT::TuneRequest:
            case MMT::SysExEnd:
            case MMT::TimingClock:
            case MMT::UndefinedRealTime1:
            case MMT::Start:
            case MMT::Continue:
            case MMT::Stop:
            case MMT::UndefinedRealTime2:
            case MMT::ActiveSensing:
            case MMT::SystemReset:
            default: break;
        }
    }

    void onSysExMessage(MIDI_Interface &, SysExMessage msg) override {
        CRTP(Derived).onSystemExclusive(msg);
    }

    void onSysCommonMessage(MIDI_Interface &, SysCommonMessage msg) override {
        using MMT = MIDIMessageType;
        switch (msg.getMessageType()) {
            case MMT::None: break;
            case MMT::NoteOff:
            case MMT::NoteOn:
            case MMT::KeyPressure:
            case MMT::ControlChange:
            case MMT::ProgramChange:
            case MMT::ChannelPressure:
            case MMT::PitchBend:
            case MMT::SysExStart: break;
            case MMT::MTCQuarterFrame:
                CRTP(Derived).onTimeCodeQuarterFrame(msg.getData1(),
                                                     msg.getCable());
                break;
            case MMT::SongPositionPointer:
                CRTP(Derived).onSongPosition(msg.getData14bit(),
                                             msg.getCable());
                break;
            case MMT::SongSelect:
                CRTP(Derived).onSongSelect(msg.getData1(), msg.getCable());
                break;
            case MMT::UndefinedSysCommon1: break;
            case MMT::UndefinedSysCommon2: break;
            case MMT::TuneRequest:
                CRTP(Derived).onTuneRequest(msg.getCable());
                break;
            case MMT::SysExEnd:
            case MMT::TimingClock:
            case MMT::UndefinedRealTime1:
            case MMT::Start:
            case MMT::Continue:
            case MMT::Stop:
            case MMT::UndefinedRealTime2:
            case MMT::ActiveSensing:
            case MMT::SystemReset:
            default: break;
        }
    }

    void onRealTimeMessage(MIDI_Interface &, RealTimeMessage msg) override {
        using MMT = MIDIMessageType;
        switch (msg.getMessageType()) {
            case MMT::None: break;
            case MMT::NoteOff:
            case MMT::NoteOn:
            case MMT::KeyPressure:
            case MMT::ControlChange:
            case MMT::ProgramChange:
            case MMT::ChannelPressure:
            case MMT::PitchBend:
            case MMT::SysExStart:
            case MMT::MTCQuarterFrame:
            case MMT::SongPositionPointer:
            case MMT::SongSelect:
            case MMT::UndefinedSysCommon1:
            case MMT::UndefinedSysCommon2:
            case MMT::TuneRequest:
            case MMT::SysExEnd: break;
            case MMT::TimingClock: CRTP(Derived).onClock(msg.getCable()); break;
            case MMT::UndefinedRealTime1: break;
            case MMT::Start: CRTP(Derived).onStart(msg.getCable()); break;
            case MMT::Continue: CRTP(Derived).onContinue(msg.getCable()); break;
            case MMT::Stop: CRTP(Derived).onStop(msg.getCable()); break;
            case MMT::UndefinedRealTime2: break;
            case MMT::ActiveSensing:
                CRTP(Derived).onActiveSensing(msg.getCable());
                break;
            case MMT::SystemReset:
                CRTP(Derived).onSystemReset(msg.getCable());
                break;
            default: break;
        }
    }

    /// @cond

    template <class...>
    struct Dummy {};

    template <class T1, class R1, class... Args1, //
              class T2, class R2, class... Args2>
    static constexpr bool same_return_type_and_arguments(R1 (T1::*)(Args1...),
                                                         R2 (T2::*)(Args2...)) {
        return std::is_same<Dummy<R1, Args1...>, Dummy<R2, Args2...>>::value;
    }

  public:
    FineGrainedMIDI_Callbacks() {
        // clang-format off
        static_assert(std::is_base_of<FineGrainedMIDI_Callbacks, Derived>::value, "Invalid CRTP");
        static_assert(same_return_type_and_arguments(&Derived::onNoteOff, &FineGrainedMIDI_Callbacks::onNoteOff), "Incorrect signature for onNoteOff");
        static_assert(same_return_type_and_arguments(&Derived::onNoteOn, &FineGrainedMIDI_Callbacks::onNoteOn), "Incorrect signature for onNoteOn");
        static_assert(same_return_type_and_arguments(&Derived::onKeyPressure, &FineGrainedMIDI_Callbacks::onKeyPressure), "Incorrect signature for onKeyPressure");
        static_assert(same_return_type_and_arguments(&Derived::onControlChange, &FineGrainedMIDI_Callbacks::onControlChange), "Incorrect signature for onControlChange");
        static_assert(same_return_type_and_arguments(&Derived::onProgramChange, &FineGrainedMIDI_Callbacks::onProgramChange), "Incorrect signature for onProgramChange");
        static_assert(same_return_type_and_arguments(&Derived::onChannelPressure, &FineGrainedMIDI_Callbacks::onChannelPressure), "Incorrect signature for onChannelPressure");
        static_assert(same_return_type_and_arguments(&Derived::onPitchBend, &FineGrainedMIDI_Callbacks::onPitchBend), "Incorrect signature for onPitchBend");
        static_assert(same_return_type_and_arguments(&Derived::onSystemExclusive, &FineGrainedMIDI_Callbacks::onSystemExclusive), "Incorrect signature for onSystemExclusive");
        static_assert(same_return_type_and_arguments(&Derived::onTimeCodeQuarterFrame, &FineGrainedMIDI_Callbacks::onTimeCodeQuarterFrame), "Incorrect signature for onTimeCodeQuarterFrame");
        static_assert(same_return_type_and_arguments(&Derived::onSongPosition, &FineGrainedMIDI_Callbacks::onSongPosition), "Incorrect signature for onSongPosition");
        static_assert(same_return_type_and_arguments(&Derived::onSongSelect, &FineGrainedMIDI_Callbacks::onSongSelect), "Incorrect signature for onSongSelect");
        static_assert(same_return_type_and_arguments(&Derived::onTuneRequest, &FineGrainedMIDI_Callbacks::onTuneRequest), "Incorrect signature for onTuneRequest");
        static_assert(same_return_type_and_arguments(&Derived::onClock, &FineGrainedMIDI_Callbacks::onClock), "Incorrect signature for onClock");
        static_assert(same_return_type_and_arguments(&Derived::onStart, &FineGrainedMIDI_Callbacks::onStart), "Incorrect signature for onStart");
        static_assert(same_return_type_and_arguments(&Derived::onContinue, &FineGrainedMIDI_Callbacks::onContinue), "Incorrect signature for onContinue");
        static_assert(same_return_type_and_arguments(&Derived::onStop, &FineGrainedMIDI_Callbacks::onStop), "Incorrect signature for onStop");
        static_assert(same_return_type_and_arguments(&Derived::onActiveSensing, &FineGrainedMIDI_Callbacks::onActiveSensing), "Incorrect signature for onActiveSensing");
        static_assert(same_return_type_and_arguments(&Derived::onSystemReset, &FineGrainedMIDI_Callbacks::onSystemReset), "Incorrect signature for onSystemReset");
        // clang-format on
    }

    /// @endcond
};

// clang-format off
template <class Derived> inline void FineGrainedMIDI_Callbacks<Derived>::onNoteOff(Channel, uint8_t, uint8_t, Cable) {}
template <class Derived> inline void FineGrainedMIDI_Callbacks<Derived>::onNoteOn(Channel, uint8_t, uint8_t, Cable) {}
template <class Derived> inline void FineGrainedMIDI_Callbacks<Derived>::onKeyPressure(Channel, uint8_t, uint8_t, Cable) {}
template <class Derived> inline void FineGrainedMIDI_Callbacks<Derived>::onControlChange(Channel, uint8_t, uint8_t, Cable) {}
template <class Derived> inline void FineGrainedMIDI_Callbacks<Derived>::onProgramChange(Channel, uint8_t, Cable) {}
template <class Derived> inline void FineGrainedMIDI_Callbacks<Derived>::onChannelPressure(Channel, uint8_t, Cable) {}
template <class Derived> inline void FineGrainedMIDI_Callbacks<Derived>::onPitchBend(Channel, uint16_t, Cable) {}
template <class Derived> inline void FineGrainedMIDI_Callbacks<Derived>::onSystemExclusive(SysExMessage) {}
template <class Derived> inline void FineGrainedMIDI_Callbacks<Derived>::onTimeCodeQuarterFrame(uint8_t,Cable) {}
template <class Derived> inline void FineGrainedMIDI_Callbacks<Derived>::onSongPosition(uint16_t,Cable) {}
template <class Derived> inline void FineGrainedMIDI_Callbacks<Derived>::onSongSelect(uint8_t,Cable) {}
template <class Derived> inline void FineGrainedMIDI_Callbacks<Derived>::onTuneRequest(Cable) {}
template <class Derived> inline void FineGrainedMIDI_Callbacks<Derived>::onClock(Cable) {}
template <class Derived> inline void FineGrainedMIDI_Callbacks<Derived>::onStart(Cable) {}
template <class Derived> inline void FineGrainedMIDI_Callbacks<Derived>::onContinue(Cable) {}
template <class Derived> inline void FineGrainedMIDI_Callbacks<Derived>::onStop(Cable) {}
template <class Derived> inline void FineGrainedMIDI_Callbacks<Derived>::onActiveSensing(Cable) {}
template <class Derived> inline void FineGrainedMIDI_Callbacks<Derived>::onSystemReset(Cable) {}
// clang-format on

END_CS_NAMESPACE
