#pragma once

#include "Bankable/MIDIButton.hpp"
#include "Bankable/MIDIButtons.hpp"
#include "MIDIButton.hpp"
#include "MIDIButtons.hpp"
#include <MIDI_Senders/DigitalNoteSender.hpp>

class NoteButton : public MIDIButton<DigitalNoteSender> {
  public:
    NoteButton(pin_t pin, uint8_t note, uint8_t channel)
        : MIDIButton(pin, note, channel) {}
};

template <uint8_t NUMBER_OF_BUTTONS>
class NoteButtons : public MIDIButtons<NUMBER_OF_BUTTONS, DigitalNoteSender> {
  public:
    NoteButtons(const ArrayWrapper<Button, NUMBER_OF_BUTTONS> &buttons,
                uint8_t baseNote, uint8_t baseChannel, uint8_t noteIncrement,
                uint8_t channelIncrement)
        : MIDIButtons<NUMBER_OF_BUTTONS, DigitalNoteSender>(
              buttons, baseNote, baseChannel, noteIncrement, channelIncrement) {
    }
};

namespace BankableNS {

class NoteButton : public MIDIButton<DigitalNoteSender> {
  public:
    NoteButton(pin_t pin, uint8_t note, uint8_t channel)
        : MIDIButton(pin, note, channel) {}
};

template <uint8_t NUMBER_OF_BUTTONS>
class NoteButtons : public MIDIButtons<NUMBER_OF_BUTTONS, DigitalNoteSender> {
  public:
    NoteButtons(const ArrayWrapper<Button, NUMBER_OF_BUTTONS> &buttons,
                uint8_t baseNote, uint8_t baseChannel, uint8_t noteIncrement,
                uint8_t channelIncrement)
        : MIDIButtons<NUMBER_OF_BUTTONS, DigitalNoteSender>(
              buttons, baseNote, baseChannel, noteIncrement, channelIncrement) {
    }
};

} // namespace BankableNS