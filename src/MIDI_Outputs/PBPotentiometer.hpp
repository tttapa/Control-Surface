#pragma once

#include <MIDI_Outputs/Abstract/MIDIFilteredAnalog.hpp>
#include <MIDI_Senders/PitchBendSender.hpp>

class PBPotentiometer : public MIDIFilteredAnalog<PitchBendSender<8>, 8> {
  public:
    PBPotentiometer(pin_t analogPin, uint8_t channel = 1)
        : MIDIFilteredAnalog<PitchBendSender<8>, 8>(analogPin, channel) {}
};