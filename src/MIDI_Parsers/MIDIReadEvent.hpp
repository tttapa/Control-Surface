#pragma once

#include <stdint.h>

#include <Settings/NamespaceSettings.hpp>
#include <AH/PrintStream/PrintStream.hpp>

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

inline Print &operator<<(Print &p, MIDIReadEvent evt) {
    switch (evt) {
        case MIDIReadEvent::NO_MESSAGE:
            return p << F("NO_MESSAGE");
        case MIDIReadEvent::CHANNEL_MESSAGE:
            return p << F("CHANNEL_MESSAGE");
        case MIDIReadEvent::SYSEX_MESSAGE:
            return p << F("SYSEX_MESSAGE");
        case MIDIReadEvent::REALTIME_MESSAGE:
            return p << F("REALTIME_MESSAGE");
        case MIDIReadEvent::SYSEX_CHUNK:
            return p << F("SYSEX_CHUNK");
        case MIDIReadEvent::SYSCOMMON_MESSAGE:
            return p << F("SYSCOMMON_MESSAGE");
        default: 
            return p << F("<Invalid MIDIReadEvent>");
    }
}

END_CS_NAMESPACE