#pragma once

#include "Bankable/MIDIButton.hpp"
#include "Bankable/MIDIButtons.hpp"
#include "MIDIButton.hpp"
#include "MIDIButtons.hpp"
#include <MIDI_Senders/DigitalCCSender.hpp>

class CCButton : public MIDIButton<DigitalCCSender> {
  public:
    CCButton(pin_t pin, uint8_t controller, uint8_t channel)
        : MIDIButton<DigitalCCSender>(pin, controller, channel) {}
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

class CCButton : public MIDIButton<DigitalCCSender> {
  public:
    CCButton(pin_t pin, uint8_t controller, uint8_t channel)
        : MIDIButton<DigitalCCSender>(pin, controller, channel) {}
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

} // namespace BankableNS