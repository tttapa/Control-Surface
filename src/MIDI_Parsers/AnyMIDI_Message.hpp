#pragma once

#include "MIDIReadEvent.hpp"
#include "MIDI_MessageTypes.hpp"
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

/// MIDI message variant type (with timestamp).
struct AnyMIDIMessage {
    MIDIReadEvent eventType = MIDIReadEvent::NO_MESSAGE;
    union Message {
        ChannelMessage channelmessage;
        SysCommonMessage syscommonmessage;
        RealTimeMessage realtimemessage;
        SysExMessage sysexmessage;

        Message() : realtimemessage(0x00) {}
        Message(ChannelMessage msg) : channelmessage(msg) {}
        Message(SysCommonMessage msg) : syscommonmessage(msg) {}
        Message(RealTimeMessage msg) : realtimemessage(msg) {}
        Message(SysExMessage msg) : sysexmessage(msg) {}
    } message;
    uint16_t timestamp = 0xFFFF;

    AnyMIDIMessage() = default;
    AnyMIDIMessage(ChannelMessage message, uint16_t timestamp)
        : eventType(MIDIReadEvent::CHANNEL_MESSAGE), message(message),
          timestamp(timestamp) {}
    AnyMIDIMessage(SysCommonMessage message, uint16_t timestamp)
        : eventType(MIDIReadEvent::SYSCOMMON_MESSAGE), message(message),
          timestamp(timestamp) {}
    AnyMIDIMessage(RealTimeMessage message, uint16_t timestamp)
        : eventType(MIDIReadEvent::REALTIME_MESSAGE), message(message),
          timestamp(timestamp) {}
    AnyMIDIMessage(SysExMessage message, uint16_t timestamp)
        : eventType(message.isLastChunk() ? MIDIReadEvent::SYSEX_MESSAGE
                                          : MIDIReadEvent::SYSEX_CHUNK),
          message(message), timestamp(timestamp) {}
};

END_CS_NAMESPACE