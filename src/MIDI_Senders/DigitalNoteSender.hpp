#pragma once

#include <Control_Surface/Control_Surface_Class.h>

class DigitalNoteSender {
  public:
    static void sendOn(uint8_t channel, uint8_t address) {
        Control_Surface.MIDI().send(NOTE_ON, channel, address, velocity);
    }
    static void sendOff(uint8_t channel, uint8_t address) {
        Control_Surface.MIDI().send(NOTE_OFF, channel, address, velocity);
    }
    constexpr static uint8_t velocity = 0x7F;
};