/**
 * Combining a transposer and a bank to switch between different scales.
 *
 * @include Other/GitHub-issues/Note-ManyAddresses-Transposer/ManyAddressBankNoteButton.hpp
 */

#include "ManyAddressBankNoteButton.hpp"

USBDebugMIDI_Interface midi;

// Transposer to go +1 to -1 octave
Transposer<-1, +1> transposer(12);
IncrementDecrementSelector<transposer.getNumberOfBanks()> transposeSelector {
  transposer,
  {A0, A1}, // Press push button A0 → +1 octave, A1 → -1 octave, A0+A1 → reset
  Wrap::Clamp,
};

// Bank to select first or second set of notes.
Bank<2> scaleBank;
IncrementDecrementSelector<2> scaleSelector {
  scaleBank,
  {A2, A3}, // Press push button A2 → second set of notes, A3 → first set
  Wrap::Clamp,
};

Bankable::ManyAddresses::ManyAddressBankNoteButton<2> buttons[] {
  {
    {scaleBank, {MIDI_Notes::F[4], MIDI_Notes::Gb[4]}},
    transposer,
    2, // pin
  },
  {
    {scaleBank, {MIDI_Notes::C[4], MIDI_Notes::Db[4]}},
    transposer,
    3, // pin
  },
  {
    {scaleBank, {MIDI_Notes::G[4], MIDI_Notes::Ab[4]}},
    transposer,
    4, // pin
  },
  // etc.
};

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
}
