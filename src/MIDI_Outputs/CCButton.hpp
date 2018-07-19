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

template <uint8_t NUMBER_OF_BUTTONS>
class CCButtons : public Abstract_Digital_CC_Output,
                  public MIDIButtons<NUMBER_OF_BUTTONS> {
  public:
    CCButtons(const Button (&buttons)[NUMBER_OF_BUTTONS], uint8_t baseAddress,
              uint8_t baseChannel, uint8_t addressIncrement,
              uint8_t channelIncrement)
        : MIDIButtons<NUMBER_OF_BUTTONS>(buttons, baseAddress, baseChannel,
                                         addressIncrement, channelIncrement) {}
};