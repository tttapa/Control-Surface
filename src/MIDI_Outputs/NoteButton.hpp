#pragma once

#include "MIDIButton.hpp"
#include <MIDI_Senders/DigitalNoteSender.hpp>

class NoteButton : public MIDIButton<DigitalNoteSender> {
  public:
    NoteButton(pin_t pin, uint8_t note, uint8_t channel)
        : MIDIButton(pin, note, channel) {}
};

class NoteButton_Bankable : public MIDIButton_Bankable<DigitalNoteSender> {
  public:
    NoteButton_Bankable(pin_t pin, uint8_t note, uint8_t channel)
        : MIDIButton_Bankable(pin, note, channel) {}
};