#pragma once

#include "Abstract_MIDI_Output_Addressable.hpp"
#include "MIDIButton.hpp"
#include <MIDI_Senders/DigitalCCSender.hpp>

class CCButton : public MIDIButton<DigitalCCSender> {
  public:
    CCButton(pin_t pin, uint8_t controller, uint8_t channel)
        : MIDIButton<DigitalCCSender>(pin, controller, channel) {}
};

class CCButton_Bankable : public MIDIButton_Bankable<DigitalCCSender> {
  public:
    CCButton_Bankable(pin_t pin, uint8_t controller, uint8_t channel)
        : MIDIButton_Bankable<DigitalCCSender>(pin, controller, channel) {}
};

template <uint8_t NUMBER_OF_BUTTONS>
class CCButtons : public MIDIButtons<NUMBER_OF_BUTTONS, DigitalCCSender> {
  public:
    CCButtons(const ArrayWrapper<Button, NUMBER_OF_BUTTONS> &buttons,
              uint8_t baseAddress, uint8_t baseChannel,
              uint8_t addressIncrement, uint8_t channelIncrement)
        : MIDIButtons<NUMBER_OF_BUTTONS, DigitalCCSender>(
              buttons, baseAddress, baseChannel, addressIncrement,
              channelIncrement) {}
};

namespace BankableNS {

template <uint8_t NUMBER_OF_BUTTONS>
class CCButtons
    : public MIDIButtons_Bankable<NUMBER_OF_BUTTONS, DigitalCCSender> {
  public:
    CCButtons(const ArrayWrapper<Button, NUMBER_OF_BUTTONS> &buttons,
              uint8_t baseAddress, uint8_t baseChannel,
              uint8_t addressIncrement, uint8_t channelIncrement)
        : MIDIButtons_Bankable<NUMBER_OF_BUTTONS, DigitalCCSender>(
              buttons, baseAddress, baseChannel, addressIncrement,
              channelIncrement) {}
};

} // namespace Bankable