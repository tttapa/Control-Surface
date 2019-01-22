/**
 * @brief   This is an example of the `CCPotentiometer` class of the 
 *          Control_Surface library.
 * 
 * Connect a potentiometer to analog pin A0. This will be the MIDI channel volume of channel 1.  
 * Map it in your DAW or DJ software.
 * 
 * Written by Pieter P, 28-08-2018  
 * https://github.com/tttapa/Control-Surface
 */

// Include the Control Surface library
#include <Control_Surface.h>

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// Create a new instance of the class `CCPotentiometer`, called `potentiometer`,
// on pin A0, that sends MIDI messages with controller 7 (channel volume)
// on channel 1.
CCPotentiometer potentiometer(A0, {MIDI_CC::Channel_Volume, CHANNEL_1});

void setup() {
    // Initialize everything
    Control_Surface.begin();
}

void loop() {
    // Update the Control Surface (check whether the potentiometer's
    // input has changed since last time, if so, send the new value over MIDI).
    Control_Surface.loop();
}