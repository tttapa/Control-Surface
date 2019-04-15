#include <Encoder.h> // Include the Encoder library.
// This must be done before the Control Surface library.
#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI interface to use for the Control Surface.
USBMIDI_Interface midi;

// Instantiate a transposer that transposes from -12 semitones to +12 semitones.
Transposer<-12, +12> transp;

// Instantiate an encoder selector with the encoder wired to pins 0 and 1
// with 4 pulses per step, without wrapping.
EncoderSelector<transp.N> sel = {transp, {0, 1}, 4, Wrap::NoWrap};

constexpr Channel channel = CHANNEL_1; // MIDI channel to send on

using namespace MIDI_Notes;
Bankable::NoteChordButton buttons[] = {
    // Major C chord in 4th octave, button between pin 2 and ground
    {transp, 2, {note(C, 4), channel}, Bass::Double + Chords::Major},
    {transp, 3, {note(D, 4), channel}, Bass::Double + Chords::Minor},
    {transp, 4, {note(E, 4), channel}, Bass::Double + Chords::Minor},
    {transp, 5, {note(F, 4), channel}, Bass::Double + Chords::MajorFirstInv},
    {transp, 6, {note(G, 4), channel}, Bass::Double + Chords::MajorSecondInv},
    {transp, 7, {note(A, 4), channel}, Bass::Double + Chords::MinorSecondInv},
    {transp, 8, {note(B, 4), channel}, Bass::Double + Chords::Diminished},
};

/*
 * You can change the chords afterwards using 
 *    buttons[0].setChord(Chords::MajorSeventh);
 * for example.
 */

void setup() {
    Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
    Control_Surface.loop(); // Update all elements
}
