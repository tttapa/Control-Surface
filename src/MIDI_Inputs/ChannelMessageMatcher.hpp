/* ✔ */

#pragma once

#include <MIDI_Parsers/MIDI_Parser.hpp>

BEGIN_CS_NAMESPACE

/// Struct for easily matching MIDI messages.
struct ChannelMessageMatcher {
    ChannelMessageMatcher(MIDIMessageType type, Channel channel, uint8_t data1,
                          uint8_t data2, uint8_t CN = 0)
        : type(type), channel(channel), data1(data1), data2(data2), CN(CN) {}
    ChannelMessageMatcher(const ChannelMessage &midimsg)
        : type(midimsg.getMessageType()), channel(midimsg.getChannel()),
          data1(midimsg.data1), data2(midimsg.data2), CN(midimsg.CN) {}
    MIDIMessageType type;
    Channel channel;
    uint8_t data1;
    uint8_t data2;
    uint8_t CN;
};

END_CS_NAMESPACE