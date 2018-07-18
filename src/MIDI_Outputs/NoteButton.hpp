#pragma once

#include "Abstract_MIDI_Output_Addressable.hpp"
#include "MIDIButton.hpp"

class NoteButton : public Abstract_Digital_Note_Output, public MIDIButton {
  public:
    NoteButton(pin_t pin, uint8_t note, uint8_t channel)
        : MIDIButton(pin, note, channel) {}
};

class NoteButton_Bankable : public Abstract_Digital_Note_Output,
                            public MIDIButton_Bankable {
  public:
    NoteButton_Bankable(pin_t pin, uint8_t note, uint8_t channel)
        : MIDIButton_Bankable(pin, note, channel) {}
};