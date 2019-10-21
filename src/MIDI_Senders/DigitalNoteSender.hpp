#pragma once

#include <Control_Surface/Control_Surface_Class.hpp>

BEGIN_CS_NAMESPACE

class DigitalNoteSender {
  public:
    DigitalNoteSender(uint8_t velocity = 0x7F) : velocity(velocity) {}
    void sendOn(MIDICNChannelAddress address) {
        Control_Surface.MIDI().sendNoteOn(address, getVelocity());
    }
    void sendOff(MIDICNChannelAddress address) {
        Control_Surface.MIDI().sendNoteOff(address, 0x7F);
    }

    void setVelocity(uint8_t velocity) { this->velocity = velocity; }
    uint8_t getVelocity() const { return this->velocity; }

  private:
    uint8_t velocity;
};

END_CS_NAMESPACE