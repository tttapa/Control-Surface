/**
 * This is the first example used in the 'Getting Started' guide:
 * https://tttapa.github.io/Control-Surface/Doc/Doxygen/dd/dcc/md_Getting-Started.html
 * 
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, Teensy 3.x, ESP32
 *
 *  Written by PieterP, 2018-08-29  
 *  https://github.com/tttapa/Control-Surface
 */

// Include the library
#include <Control_Surface.h>

// Instantiate a MIDI Interface to use
USBMIDI_Interface midi;

// Instantiate an analog multiplexer
CD74HC4051 mux {
  A0,       // Analog input pin
  {3, 4, 5} // Address pins S0, S1, S2
};

// Create an array of potentiometers that send out
// MIDI Control Change messages when you turn the
// potentiometers connected to the eight input pins of
// the multiplexer
CCPotentiometer volumePotentiometers[] {
  {mux.pin(0), {MIDI_CC::Channel_Volume, CHANNEL_1}},
  {mux.pin(1), {MIDI_CC::Channel_Volume, CHANNEL_2}},
  {mux.pin(2), {MIDI_CC::Channel_Volume, CHANNEL_3}},
  {mux.pin(3), {MIDI_CC::Channel_Volume, CHANNEL_4}},
  {mux.pin(4), {MIDI_CC::Channel_Volume, CHANNEL_5}},
  {mux.pin(5), {MIDI_CC::Channel_Volume, CHANNEL_6}},
  {mux.pin(6), {MIDI_CC::Channel_Volume, CHANNEL_7}},
  {mux.pin(7), {MIDI_CC::Channel_Volume, CHANNEL_8}},
};

// Initialize the Control Surface
void setup() {
  Control_Surface.begin();
}

// Update the Control Surface
void loop() {
  Control_Surface.loop();
}
