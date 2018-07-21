#pragma once

#include <Control_Surface/Control_Surface_Class.h>

class ContinuousCCSender {
  public:
    static void send(uint8_t value, uint8_t channel, uint8_t address) {
        Control_Surface.MIDI().send(CC, channel, address, value);
    }
};