#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI interface to use for the Control Surface.
USBMIDI_Interface midi;

// Instantiate a transposer that transposes from -12 semitones to +12 semitones.
Transposer<-12, +12> transposer;

// Instantiate a button on pin 4 sending note C4 (0x3C) on channel 1
Bankable::NoteButton note = { transposer, 4, 0x3C, 1 };

// Instantiate a selector for the program changer on pins 5 (increment) and
// pin 6 (decrement). It doesn't wrap around (false).
IncrementDecrementSelector<25> sel = { transposer, {5, 6}, false };

void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop();
}
