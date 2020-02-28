#pragma once

#include <AH/Math/IncreaseBitDepth.hpp>
#include <Control_Surface/Control_Surface_Class.hpp>

BEGIN_CS_NAMESPACE

class ContinuousCCSender {
  public:
    void send(uint8_t value, MIDIAddress address) {
        Control_Surface.MIDI().sendCC(address, value);
    }

    constexpr static uint8_t precision() { return 7; }
};

template <uint8_t INPUT_PRECISION_BITS>
class ContinuousCCSender14 {
  public:
    void send(uint16_t value, MIDIAddress address) {
        value =
            AH::increaseBitDepth<14, precision(), uint16_t, uint16_t>(value);
        Control_Surface.MIDI().sendCC(address + 0x00, (value >> 7) & 0x7f);
        Control_Surface.MIDI().sendCC(address + 0x20, (value >> 0) & 0x7F);
    }

    constexpr static uint8_t precision() {
        static_assert(INPUT_PRECISION_BITS <= 14,
                      "Maximum resolution is 14 bits");
        return INPUT_PRECISION_BITS;
    }
};

END_CS_NAMESPACE