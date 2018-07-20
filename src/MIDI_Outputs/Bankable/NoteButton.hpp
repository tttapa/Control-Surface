#pragma once

#include <MIDI_Outputs/Bankable/Abstract/MIDIButton.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

namespace Bankable {

class NoteButton : public MIDIButton<DigitalNoteSender> {
  public:
    NoteButton(pin_t pin, uint8_t note, uint8_t channel)
        : MIDIButton(pin, note, channel) {}
};

} // namespace Bankable