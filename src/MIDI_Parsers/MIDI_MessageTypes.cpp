#include "MIDI_MessageTypes.hpp"
#include "Settings/NamespaceSettings.hpp"

BEGIN_CS_NAMESPACE

FlashString_t enum_to_string(MIDIMessageType m) {
    using M = MIDIMessageType;
    switch (m) {
        case M::None: return F("None");
        case M::NoteOff: return F("NoteOff");
        case M::NoteOn: return F("NoteOn");
        case M::KeyPressure: return F("KeyPressure");
        case M::ControlChange: return F("ControlChange");
        case M::ProgramChange: return F("ProgramChange");
        case M::ChannelPressure: return F("ChannelPressure");
        case M::PitchBend: return F("PitchBend");
        case M::SysExStart: return F("SysExStart");
        case M::MTCQuarterFrame: return F("MTCQuarterFrame");
        case M::SongPositionPointer: return F("SongPositionPointer");
        case M::SongSelect: return F("SongSelect");
        case M::UndefinedSysCommon1: return F("UndefinedSysCommon1");
        case M::UndefinedSysCommon2: return F("UndefinedSysCommon2");
        case M::TuneRequest: return F("TuneRequest");
        case M::SysExEnd: return F("SysExEnd");
        case M::TimingClock: return F("TimingClock");
        case M::UndefinedRealTime1: return F("UndefinedRealTime1");
        case M::Start: return F("Start");
        case M::Continue: return F("Continue");
        case M::Stop: return F("Stop");
        case M::UndefinedRealTime2: return F("UndefinedRealTime2");
        case M::ActiveSensing: return F("ActiveSensing");
        case M::SystemReset: return F("SystemReset");
        default: return F("<invalid>");
    }
}

END_CS_NAMESPACE