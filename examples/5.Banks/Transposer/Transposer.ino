/** 
 * This is an example that demonstrates the use a Transposer to transpose the
 * note events sent out by NoteButton%s.
 *
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 * - 2-9: momentary push buttons (to ground) to use as "piano keys"
 * - 10: momentary push button (to ground) to transpose one semitone up
 * - 11: momentary push button (to ground) to transpose one semitone down
 * 
 * The internal pull-up resistor for the buttons will be enabled automatically.
 * 
 * Behavior
 * --------
 * 
 * - By default, the 8 push buttons on pins 2-9 send MIDI Note events for notes
 *   C4 - C5.
 * - If you press the push button on pin 10, all notes are transposed one 
 *   semitone up.
 * - If you press the push button on pin 11, all notes are transposed one 
 *   semitone down.
 * - If you press the push buttons on pins 10 and 11 simultaneously, the 
 *   transposer resets.
 * 
 * Mapping
 * -------
 * 
 * Route the Arduino MIDI port into a synth to play the notes.
 * 
 * Written by PieterP, 2019-10-26  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// Instantiate a Transposer that can transpose from one octave down to one
// octave up
Transposer<-12, +12> transposer;

// Instantiate a Selector to change the transposition
IncrementDecrementSelector<transposer.getNumberOfBanks()> selector {
  transposer,
  {10, 11},
  Wrap::Clamp,
};

// Instantiate an array of NoteButton objects
Bankable::NoteButton buttons[] {
  {transposer, 2, MIDI_Notes::C(4)}, {transposer, 3, MIDI_Notes::D(4)},
  {transposer, 4, MIDI_Notes::E(4)}, {transposer, 5, MIDI_Notes::F_(4)},
  {transposer, 6, MIDI_Notes::G(4)}, {transposer, 7, MIDI_Notes::A(4)},
  {transposer, 8, MIDI_Notes::B(4)}, {transposer, 9, MIDI_Notes::C(5)},
};

void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface
}