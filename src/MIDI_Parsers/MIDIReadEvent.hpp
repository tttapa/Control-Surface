#pragma once

#include <stdint.h>

#include <AH/PrintStream/PrintStream.hpp>
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

/// Values returned by the MIDI reading functions.
enum class MIDIReadEvent : uint8_t {
    NO_MESSAGE = 0,        ///< No new messages were received.
    CHANNEL_MESSAGE = 1,   ///< A MIDI Channel message was received.
    SYSEX_MESSAGE = 2,     ///< A MIDI System Exclusive message was received.
    REALTIME_MESSAGE = 3,  ///< A MIDI Real-Time message was received.
    SYSEX_CHUNK = 4,       ///< An incomplete System Exclusive message.
    SYSCOMMON_MESSAGE = 5, ///< A MIDI System Common message was received.
};

inline FlashString_t enum_to_string(MIDIReadEvent evt) {
    switch (evt) {
        case MIDIReadEvent::NO_MESSAGE: return F("NO_MESSAGE");
        case MIDIReadEvent::CHANNEL_MESSAGE: return F("CHANNEL_MESSAGE");
        case MIDIReadEvent::SYSEX_MESSAGE: return F("SYSEX_MESSAGE");
        case MIDIReadEvent::REALTIME_MESSAGE: return F("REALTIME_MESSAGE");
        case MIDIReadEvent::SYSEX_CHUNK: return F("SYSEX_CHUNK");
        case MIDIReadEvent::SYSCOMMON_MESSAGE: return F("SYSCOMMON_MESSAGE");
        default: return F("<invalid>");
    }
}

inline Print &operator<<(Print &p, MIDIReadEvent evt) {
    return p << enum_to_string(evt);
}

END_CS_NAMESPACE