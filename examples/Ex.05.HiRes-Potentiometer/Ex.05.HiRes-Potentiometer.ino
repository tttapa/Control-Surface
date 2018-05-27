/**
 * @example Ex.05.HiRes-Potentiometer.ino 
 * 
 * This is an example of the AnalogPB class of the Control_Surface library.
 * 
 * Connect a potentiometer to analog pin A0. It will send MIDI Pitch Bend messages on channel 1.  
 * Map it in your DAW or DJ software.
 * 
 * Written by Pieter P, 08-09-2017  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the library

// Create a new instance of the class 'AnalogPB', called 'potentiometer', on pin A0, 
// that sends MIDI Pitch Bend messages on channel 1
AnalogPB potentiometer(A0, 1);

void setup() {}

void loop() {
  // Refresh the MIDI controller (check whether the potentiometer's input has changed since last time, if so, send the new value over MIDI)
  Control_Surface.refresh();
}