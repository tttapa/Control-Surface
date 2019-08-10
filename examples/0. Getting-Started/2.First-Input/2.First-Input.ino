/**
 * This is the second example used in the 'Getting Started' guide:
 * https://tttapa.github.io/Control-Surface/Doc/Doxygen/dd/dcc/md_Getting-Started.html
 * 
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 *
 * Written by PieterP, 2018-08-29  
 * https://github.com/tttapa/Control-Surface
 */

// Include the library
#include <Control_Surface.h>

// Instantiate a MIDI Interface to use
USBMIDI_Interface midi;

// Instantiate an analog multiplexer
SPIShiftRegisterOut<8> sreg = {10, MSBFIRST};

using namespace MIDI_Notes;

// Create an array of LEDs that listen to MIDI Note messages, turning on and off
// the LEDs connected to the eight input pins of the shift register
MIDINoteLED leds[] = {
  {sreg.pin(0), note(C, 4)}, {sreg.pin(1), note(D, 4)},
  {sreg.pin(2), note(E, 4)}, {sreg.pin(3), note(F, 4)},
  {sreg.pin(4), note(G, 4)}, {sreg.pin(5), note(A, 4)},
  {sreg.pin(6), note(B, 4)}, {sreg.pin(7), note(C, 5)},
};

// Initialize the Control Surface
void setup() {
  Control_Surface.begin();
}

// Update the Control Surface
void loop() {
  Control_Surface.loop();
}
