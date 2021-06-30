#include "MIDI_MessageTypes.hpp"
#include "Settings/NamespaceSettings.hpp"

BEGIN_CS_NAMESPACE

FlashString_t enum_to_string(MIDIMessageType m) {
    using M = MIDIMessageType;
    switch (m) {
        case M::NONE: return F("NONE");
        case M::NOTE_OFF: return F("NOTE_OFF");
        case M::NOTE_ON: return F("NOTE_ON");
        case M::KEY_PRESSURE: return F("KEY_PRESSURE");
        case M::CONTROL_CHANGE: return F("CONTROL_CHANGE");
        case M::PROGRAM_CHANGE: return F("PROGRAM_CHANGE");
        case M::CHANNEL_PRESSURE: return F("CHANNEL_PRESSURE");
        case M::PITCH_BEND: return F("PITCH_BEND");
        case M::SYSEX_START: return F("SYSEX_START");
        case M::MTC_QUARTER_FRAME: return F("MTC_QUARTER_FRAME");
        case M::SONG_POSITION_POINTER: return F("SONG_POSITION_POINTER");
        case M::SONG_SELECT: return F("SONG_SELECT");
        case M::UNDEFINED_SYSCOMMON_1: return F("UNDEFINED_SYSCOMMON_1");
        case M::UNDEFINED_SYSCOMMON_2: return F("UNDEFINED_SYSCOMMON_2");
        case M::TUNE_REQUEST: return F("TUNE_REQUEST");
        case M::SYSEX_END: return F("SYSEX_END");
        case M::TIMING_CLOCK: return F("TIMING_CLOCK");
        case M::UNDEFINED_REALTIME_1: return F("UNDEFINED_REALTIME_1");
        case M::START: return F("START");
        case M::CONTINUE: return F("CONTINUE");
        case M::STOP: return F("STOP");
        case M::UNDEFINED_REALTIME_2: return F("UNDEFINED_REALTIME_2");
        case M::ACTIVE_SENSING: return F("ACTIVE_SENSING");
        case M::SYSTEM_RESET: return F("SYSTEM_RESET");
        default: return F("<invalid>");
    }
}

END_CS_NAMESPACE