#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI interface to use for the Control Surface.
USBMIDI_Interface midi;

// Instantiate a transposer that transposes from -12 semitones to +12 semitones.
Transposer<-12, +12> transposer;

// Instantiate a button on pin 4 sending note C3 (0x3C) on channel 1
Bankable::NoteButton notebtn = {transposer, 4, {0x3C, CHANNEL_1}};

IncrementDecrementSelector<transposer.N> sel = {
    transposer,
    {5, 6},
    Wrap::NoWrap,
};

void setup() {
    Control_Surface.begin(); // Initialize Control Surface
}

void loop() { Control_Surface.loop(); }
