#pragma once

#include <Control_Surface/Control_Surface_Class.h>

class ContinuousCCSender {
  public:
    static void send(uint8_t value, MIDICNChannelAddress address) {
        Control_Surface.MIDI().sendCC(address, value);
    }
};