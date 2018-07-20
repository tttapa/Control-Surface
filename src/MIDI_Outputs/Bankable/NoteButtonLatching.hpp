#pragma once

#include <MIDI_Outputs/Bankable/Abstract/MIDIButtonLatching.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

namespace Bankable {

class NoteButtonLatching : public MIDIButtonLatching<DigitalNoteSender> {
  public:
    NoteButtonLatching(pin_t pin, uint8_t note, uint8_t channel)
        : MIDIButtonLatching(pin, note, channel) {}
};

} // namespace Bankable