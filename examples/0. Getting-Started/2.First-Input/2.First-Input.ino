/**
 * This is the second example used in the 'Getting Started' guide:
 * https://tttapa.github.io/Control-Surface/Doc/Doxygen/dd/dcc/md_Getting-Started.html
 * 
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, Teensy 3.x, ESP32
 * 
 * @htmlonly
 * <iframe width="560" height="315"
 * src="https://www.youtube.com/embed/u1IbYXPT6mM?loop=1" frameborder="0"
 * allow="accelerometer; autoplay; encrypted-media; gyroscope;
 * picture-in-picture" allowfullscreen></iframe>
 * @endhtmlonly
 *
 * Written by PieterP, 2018-08-29  
 * https://github.com/tttapa/Control-Surface
 */

// Include the library
#include <Control_Surface.h>

// Instantiate a MIDI Interface to use
USBMIDI_Interface midi;

// Instantiate a shift register as output for the LEDs
SPIShiftRegisterOut<8> sreg {
  SPI,      // SPI interface to use
  10,       // Latch pin (ST_CP)
  MSBFIRST, // Byte order
};

// Create an array of LEDs that listen to MIDI Note messages, turning on and off
// the LEDs connected to the eight output pins of the shift register
NoteLED leds[] {
  {sreg.pin(0), MIDI_Notes::C(4)},  // LED pin, address (note number, channel, cable)
  {sreg.pin(1), MIDI_Notes::D(4)},  //
  {sreg.pin(2), MIDI_Notes::E(4)},  //
  {sreg.pin(3), MIDI_Notes::F_(4)}, // F is an exception :(
  {sreg.pin(4), MIDI_Notes::G(4)},  //
  {sreg.pin(5), MIDI_Notes::A(4)},  //
  {sreg.pin(6), MIDI_Notes::B(4)},  //
  {sreg.pin(7), MIDI_Notes::C(5)},  //
};

// Initialize the Control Surface
void setup() {
  Control_Surface.begin();
}

// Update the Control Surface
void loop() {
  Control_Surface.loop();
}
