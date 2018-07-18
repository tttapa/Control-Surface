#pragma once

#include "DigitalMIDIOut.hpp"
#include <Control_Surface/Control_Surface_Class.h>

class DigitalCCOut : public virtual DigitalMIDIOut {
  public:
    DigitalCCOut(uint8_t offValue, uint8_t onValue)
        : offValue(offValue), onValue(onValue) {}

    void sendOn() const override {
        Control_Surface.MIDI().send(CC, getChannel(), getAddress(), onValue);
    }
    void sendOff() const override {
        Control_Surface.MIDI().send(CC, getChannel(), getAddress(), offValue);
    }

  private:
    const uint8_t offValue;
    const uint8_t onValue;
};