#pragma once

#include "Bankable/MIDIButtonLatching.hpp"
//#include "Bankable/MIDIButtonsLatching.hpp"
#include "MIDIButtonLatching.hpp"
//#include "MIDIButtonsLatching.hpp"
#include <MIDI_Senders/DigitalNoteSender.hpp>

// TODO: these are all in one file because I'm lazy

class NoteButtonLatching : public MIDIButtonLatching<DigitalNoteSender> {
  public:
    NoteButtonLatching(pin_t pin, uint8_t note, uint8_t channel)
        : MIDIButtonLatching(pin, note, channel) {}
};

/*
TODO
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
*/

namespace Bankable {

class NoteButtonLatching : public MIDIButtonLatching<DigitalNoteSender> {
  public:
    NoteButtonLatching(pin_t pin, uint8_t note, uint8_t channel)
        : MIDIButtonLatching(pin, note, channel) {}
};

/*
TODO
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
*/

} // namespace Bankable