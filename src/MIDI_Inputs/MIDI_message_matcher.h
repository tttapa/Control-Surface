#ifndef MIDI_MESSAGE_MATCHER_H
#define MIDI_MESSAGE_MATCHER_H

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdint>
#endif
#include "../MIDI_Parsers/MIDI_Parser.h"

struct MIDI_message_matcher
{
    MIDI_message_matcher(uint8_t type, uint8_t channel, uint8_t data1, uint8_t data2)
        : type(type),
          channel(channel),
          data1(data1),
          data2(data2) {}
    MIDI_message_matcher(const MIDI_message &midimsg)
        : type(midimsg.header & 0xF0),
          channel(midimsg.header & 0x0F),
          data1(midimsg.data1),
          data2(midimsg.data2) {}
    uint8_t type;
    uint8_t channel;
    uint8_t data1;
    uint8_t data2;
};

#endif // MIDI_MESSAGE_MATCHER_H