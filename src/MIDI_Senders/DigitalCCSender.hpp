#pragma once

#include <Control_Surface/Control_Surface_Class.hpp>

BEGIN_CS_NAMESPACE

class DigitalCCSender {
  public:
    DigitalCCSender(uint8_t onValue = 0x7F, uint8_t offValue = 0x00)
        : onValue{onValue}, offValue{offValue} {}

    void sendOn(MIDICNChannelAddress address) {
        Control_Surface.MIDI().sendCC(address, onValue);
    }
    void sendOff(MIDICNChannelAddress address) {
        Control_Surface.MIDI().sendCC(address, offValue);
    }

    uint8_t getOnValue() const { return this->onValue; }
    void setOnValue(uint8_t onValue) { this->onValue = onValue; }
    uint8_t getOffValue() const { return this->offValue; }
    void setOffValue(uint8_t offValue) { this->offValue = offValue; }

  private:
    uint8_t onValue;
    uint8_t offValue;
};

END_CS_NAMESPACE