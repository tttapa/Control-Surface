#pragma once

#include <AH/Math/IncreaseBitDepth.hpp>
#include <Control_Surface/Control_Surface_Class.hpp>

BEGIN_CS_NAMESPACE

template <uint8_t INPUT_PRECISION_BITS>
class PitchBendSender {
  public:
    static void send(uint16_t value, MIDICNChannelAddress address) {
        value =
            AH::increaseBitDepth<14, precision(), uint16_t, uint16_t>(value);
        // ignore address byte, just use channel and cable numbers
        MIDICNChannel channelCN = {address.getChannel(),
                                   address.getCableNumber()};
        Control_Surface.MIDI().sendPB(channelCN, value);
    }

    constexpr static uint8_t precision() {
        static_assert(INPUT_PRECISION_BITS <= 14,
                      "Maximum pitch bend resolution is 14 bits");
        return INPUT_PRECISION_BITS;
    }
};

END_CS_NAMESPACE