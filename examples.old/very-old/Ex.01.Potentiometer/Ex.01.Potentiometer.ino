/**
 * @example Ex.01.Potentiometer.ino
 * 
 * This is an example of the `CCPotentiometer` class of the Control_Surface library.
 * 
 * Connect a potentiometer to analog pin A0. This will be the MIDI channel volume of channel 1.  
 * Map it in your DAW or DJ software.
 * 
 * Written by Pieter P, 08-09-2017  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the library

// Create a new instance of the class `CCPotentiometer`, called `potentiometer`,
// on pin A0, that sends MIDI messages with controller 7 (channel volume)
// on channel 1
CCPotentiometer potentiometer(A0, MIDI_CC::Channel_Volume, 1);

void setup() {}

void loop() {
    // Update the Control Surface (check whether the potentiometer's
    // input has changed since last time, if so, send the new value over MIDI).
    Control_Surface.update();
}
