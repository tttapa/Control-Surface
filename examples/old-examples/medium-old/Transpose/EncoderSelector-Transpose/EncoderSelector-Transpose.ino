#include <Encoder.h> // Include the Encoder library.
// This must be done before the Control Surface library.
#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI interface to use for the Control Surface.
USBMIDI_Interface midi;

// Instantiate a transposer that transposes from -12 semitones to +12 semitones.
Transposer<-12, +12> transposer;

// Instantiate a button on pin 5 sending note C3 (0x3C) on channel 1
Bankable::NoteButton notebtn = {transposer, 5, {0x3C, CHANNEL_1}};

// Instantiate an encoder selector with the encoder wired to pins 2 and 3 and
// the switch pin connected to pin 4, with 4 pulses per step, without wrapping.
EncoderSelector<transposer.N> sel = {
    transposer,
    {2, 3, 4},
    4,
    Wrap::NoWrap,
};

void setup() {
    Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
    Control_Surface.loop(); // Update all elements
}
