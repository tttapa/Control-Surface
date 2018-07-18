#pragma once

#include "Abstract_MIDI_Output_Addressable.hpp"
#include "MIDIButton.hpp"

class CCButton : public Abstract_Digital_CC_Output, public MIDIButton {
  public:
    CCButton(pin_t pin, uint8_t controller, uint8_t channel)
        : MIDIButton(pin, controller, channel) {}
};

class CCButton_Bankable : public Abstract_Digital_CC_Output,
                          public MIDIButton_Bankable {
  public:
    CCButton_Bankable(pin_t pin, uint8_t controller, uint8_t channel)
        : MIDIButton_Bankable(pin, controller, channel) {}
};