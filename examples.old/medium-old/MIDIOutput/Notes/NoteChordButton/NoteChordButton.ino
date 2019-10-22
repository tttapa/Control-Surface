/**
 * @brief   This is an example of the `NoteChordButton` class of the 
 *          Control_Surface library.  
 * 
 * Connect 7 momentary push buttons between pins 2-8 and ground. These will play
 * chords when pressed.
 * 
 * Connect a rotary encoder to pins 0 & 1. This will transpose the MIDI chords.
 * 
 * Written by Pieter P, 15-04-2019  
 * https://github.com/tttapa/Control-Surface
 */

#include <Encoder.h> // Include the Encoder library.
// This must be done before the Control Surface library.
#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI interface to use for the Control Surface.
USBMIDI_Interface midi;

constexpr Channel channel = CHANNEL_1; // MIDI channel to send to

// Instantiate a transposer that transposes from -12 semitones to +12 semitones.
Transposer<-12, +12> transp;

// Instantiate an encoder selector with the encoder wired to pins 0 and 1
// with 4 pulses per step, without wrapping.
EncoderSelector<transp.N> sel = {transp, {0, 1}, 4, Wrap::NoWrap};

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
You can also use the non-bankable version without the transposer.

NoteChordButton buttons[] = {
    // Major C chord in 4th octave, button between pin 2 and ground
    {2, {note(C, 4), channel}, Bass::Double + Chords::Major},
    {3, {note(D, 4), channel}, Bass::Double + Chords::Minor},
    {4, {note(E, 4), channel}, Bass::Double + Chords::Minor},
    {5, {note(F, 4), channel}, Bass::Double + Chords::MajorFirstInv},
    {6, {note(G, 4), channel}, Bass::Double + Chords::MajorSecondInv},
    {7, {note(A, 4), channel}, Bass::Double + Chords::MinorSecondInv},
    {8, {note(B, 4), channel}, Bass::Double + Chords::Diminished},
};
*/

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
