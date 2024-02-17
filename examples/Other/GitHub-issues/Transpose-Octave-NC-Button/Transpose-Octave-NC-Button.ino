/**
 * <https://github.com/tttapa/Control-Surface/issues/74>
 */

#include <Control_Surface.h>

USBMIDI_Interface midi;

// Transposer that transposes one octave (12 semitones) at a time,
// from 0 to +1 octave.
Transposer<0, +1> transposer(12);

// Push button on pin 2 to transpose.
IncrementSelector<2> selector {transposer, 2};

Bankable::NoteButton notebutton {
  transposer,       // bank/transposer
  3,                // pin
  MIDI_Notes::C[4], // address/note
};

void setup() {
  selector.invert(); // normally closed button
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
}