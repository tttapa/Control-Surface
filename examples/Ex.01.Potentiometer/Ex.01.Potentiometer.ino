/*
This is an example of the "Analog" class of the MIDI_controller library.
Connect a potentiometer to analog pin A0. This will be the MIDI channel volume of channel 1.
Map it in your DAW or DJ software.

Written by Pieter P, 08-09-2017
https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_Controller.h> // Include the library

// Create a new instance of the class 'Analog', called 'potentiometer', on pin A0, 
// that sends MIDI messages with controller 7 (channel volume) on channel 1
Analog potentiometer(A0, MIDI_CC::Channel_Volume, 1);

void setup() {}

void loop() {
  // Refresh the MIDI controller (check whether the potentiometer's input has changed since last time, if so, send the new value over MIDI)
  MIDI_Controller.refresh();
}
