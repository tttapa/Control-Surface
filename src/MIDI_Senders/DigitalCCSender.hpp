#pragma once

#include <Control_Surface/Control_Surface_Class.hpp>

class DigitalCCSender {
  public:
    static void sendOn(MIDICNChannelAddress address) {
        Control_Surface.MIDI().sendCC(address, onValue);
    }
    static void sendOff(MIDICNChannelAddress address) {
        Control_Surface.MIDI().sendCC(address, offValue);
    }
    constexpr static uint8_t onValue = 127;
    constexpr static uint8_t offValue = 0;
};