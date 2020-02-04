#pragma once

#include <Control_Surface/Control_Surface_Class.hpp>

BEGIN_CS_NAMESPACE

class ProgramChangeSender {
  public:
    void sendOn(MIDIAddress address) {
        Control_Surface.MIDI().sendPC(address);
    }
    void sendOff(MIDIAddress address) {
        (void) address;
    }
};

END_CS_NAMESPACE