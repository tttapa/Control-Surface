#pragma once

#include <Control_Surface/Control_Surface_Class.h>

class DigitalCCSender {
    public:
    static void sendOn(uint8_t channel, uint8_t address) {
        Control_Surface.MIDI().send(CC, channel, address, onValue);
    }
    static void sendOff(uint8_t channel, uint8_t address) {
        Control_Surface.MIDI().send(CC, channel, address, offValue);
    }
    constexpr static uint8_t onValue = 127;
    constexpr static uint8_t offValue = 0;
};