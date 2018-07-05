/**
 * @example Ex.12.AnalogMultiplex.ino
 * 
 * This is an example of the AnalogMultiplex class of the Control_Surface library.
 * 
 * Connect an analog multiplexer to analog pin A0. Connect the address lines to pins 2, 3 and 4.  
 * Connect 8 potentiometers to the inputs of the multiplexer.  
 * These will be the channel volumes
 * of MIDI channels 1 through 8.  
 * Map accordingly in your DAW or DJ software.
 *
 * Written by Pieter P, 20-09-2017  
 * https://github.com/tttapa/Control-Surface
*/

#include <Control_Surface.h> // Include the library

// Create an instance of 'AnalogMultiplex' with the output pin of the multiplexer connected to
// analog input pin A0 and the address pins connected to pins 2, 3 and 4.
AnalogMultiplex multiplexer(A0, { 2, 3, 4 } );

// Create 8 new instances of the class 'Analog', on the 8 pins of the multiplexer,
// that send MIDI messages with controller 7 (channel volume) on channels 1 - 8
Analog potentiometers[] = {
  { multiplexer.pin(0), MIDI_CC::Channel_Volume, 1 },
  { multiplexer.pin(1), MIDI_CC::Channel_Volume, 2 },
  { multiplexer.pin(2), MIDI_CC::Channel_Volume, 3 },
  { multiplexer.pin(3), MIDI_CC::Channel_Volume, 4 },
  { multiplexer.pin(4), MIDI_CC::Channel_Volume, 5 },
  { multiplexer.pin(5), MIDI_CC::Channel_Volume, 6 },
  { multiplexer.pin(6), MIDI_CC::Channel_Volume, 7 },
  { multiplexer.pin(7), MIDI_CC::Channel_Volume, 8 }
};

void setup() {}

void loop() {
  // Update the MIDI controller (check whether the potentiometer's input has changed since last time, if so, send the new value over MIDI)
  Control_Surface.update();
}
