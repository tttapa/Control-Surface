#pragma once

#include <Control_Surface/Control_Surface_Class.h>

template <uint8_t INPUT_PRECISION_BITS>
class PitchBendSender {
  public:
    static void send(uint16_t value, uint8_t channel) {
        static_assert(INPUT_PRECISION_BITS >= 7,
                      "Error: pitch bend input precision must be greater than "
                      "or equal to seven.");
        // Convert to 14-bit number
        value = (value << (14 - INPUT_PRECISION_BITS)) |
                (value >> (INPUT_PRECISION_BITS - (14 - INPUT_PRECISION_BITS)));
        Control_Surface.MIDI().send(PITCH_BEND, channel, value & 0x7F,
                                    value >> 7);
    }
};