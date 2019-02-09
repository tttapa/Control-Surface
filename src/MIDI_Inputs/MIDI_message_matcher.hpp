/* ✔ */

#pragma once

#include <MIDI_Parsers/MIDI_Parser.hpp>

/// Struct for easily matching MIDI messages.
struct MIDI_message_matcher {
    MIDI_message_matcher(uint8_t type, Channel channel, uint8_t data1,
                         uint8_t data2, uint8_t CN = 0)
        : type(type), channel(channel.getRaw()), data1(data1), data2(data2),
          CN(CN) {}
    MIDI_message_matcher(const MIDI_message &midimsg)
        : type(midimsg.header & 0xF0), channel(midimsg.header & 0x0F),
          data1(midimsg.data1), data2(midimsg.data2), CN(midimsg.CN) {}
    uint8_t type;
    uint8_t channel;
    uint8_t data1;
    uint8_t data2;
    uint8_t CN;
};