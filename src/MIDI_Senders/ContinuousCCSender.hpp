#pragma once

#include <Control_Surface/Control_Surface_Class.hpp>

BEGIN_CS_NAMESPACE

class ContinuousCCSender {
  public:
    void send(uint8_t value, MIDICNChannelAddress address) {
        Control_Surface.MIDI().sendCC(address, value);
    }

    constexpr static uint8_t precision() { return 7; }
};

END_CS_NAMESPACE