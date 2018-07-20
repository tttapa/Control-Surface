#pragma once

#include <MIDI_Outputs/Bankable/Abstract/MIDIButtons.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

namespace Bankable {

template <uint8_t NUMBER_OF_BUTTONS>
class NoteButtons : public MIDIButtons<NUMBER_OF_BUTTONS, DigitalNoteSender> {
  public:
    NoteButtons(const Array<Button, NUMBER_OF_BUTTONS> &buttons,
                uint8_t baseNote, uint8_t baseChannel, uint8_t noteIncrement,
                uint8_t channelIncrement)
        : MIDIButtons<NUMBER_OF_BUTTONS, DigitalNoteSender>(
              buttons, baseNote, baseChannel, noteIncrement, channelIncrement) {
    }
};

} // namespace Bankable