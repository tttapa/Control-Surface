/*
This is an example of the "Analog" class of the MIDI_controller library.
Connect a potentiometer to analog pin A0. This will be the MIDI channel volume of channel 1.

Map it in your DAW or DJ software.

Set 'minimumValue' to the analog value read when the potentiometer is in the minimum position.
Set 'maximumValue' to the analog value read when the potentiometer is in the maximum position.

Written by Pieter P, 20-09-2017
https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_Controller.h> // Include the library

// Create a new instance of the class 'Analog', called 'potentiometer', on pin A0, 
// that sends MIDI messages with controller 7 (channel volume) on channel 1
Analog potentiometer(A0, MIDI_CC::Channel_Volume, 1);

const int minimumValue = 3;    // the analog value read when the potentiometer is in the minimum position.
const int maximumValue = 1020; // the analog value read when the potentiometer is in the maximum position.

int mapCalibrated(int value) {
  value = constrain(value, minimumValue, maximumValue); // make sure that the analog value is between the minimum and maximum value
  return map(value, minimumValue, maximumValue, 0, 1023); // map the value from [minimumValue, maximumValue] to [0, 1023]
}

void setup() {
  potentiometer.map(mapCalibrated); // apply the 'mapCalibrated' function on the analog input of 'potentiometer'
}

void loop() {
  // Refresh the MIDI controller (check whether the potentiometer's input has changed since last time, if so, send the new value over MIDI)
  MIDI_Controller.refresh();
}
