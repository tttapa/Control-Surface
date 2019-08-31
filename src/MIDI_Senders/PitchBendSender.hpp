#pragma once

#include <Control_Surface/Control_Surface_Class.hpp>
#include <Helpers/IncreaseBitDepth.hpp>

template <uint8_t INPUT_PRECISION_BITS>
class PitchBendSender {
  public:
    static void send(uint16_t value, MIDICNChannelAddress address) {
        value = increaseBitDepth<14, INPUT_PRECISION_BITS, uint16_t, uint16_t>(
            value);
        Control_Surface.MIDI().sendPB(address, value);
    }
};