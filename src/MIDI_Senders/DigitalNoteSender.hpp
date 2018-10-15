#pragma once

#include <Control_Surface/Control_Surface_Class.hpp>

class DigitalNoteSender {
  public:
    static void sendOn(MIDICNChannelAddress address) {
        Control_Surface.MIDI().sendNoteOn(address, velocity);
    }
    static void sendOff(MIDICNChannelAddress address) {
        Control_Surface.MIDI().sendNoteOff(address, velocity);
    }
    constexpr static uint8_t velocity = 0x7F;
};