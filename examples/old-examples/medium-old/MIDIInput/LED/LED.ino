/**
 * @brief   This example demonstrates the use of LEDs that respond to MIDI input.
 * 
 * If a MIDI Note On event for note 0x3C (middle C) is sent, the built-in LED
 * will light up, if a Note Off event for that note is sent, the LED will turn
 * off.
 * 
 * Written by PieterP, 07-02-2019  
 * https://github.com/tttapa/Control-Surface
 */

// Include the Control Surface library
#include <Control_Surface.h>

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

using namespace MIDI_Notes;

// Instantiate the LED that will light up when middle C is playing
MIDINoteLED led = {LED_BUILTIN, {note(C, 4), CHANNEL_1}};

void setup() {
    // Initialize everything
    Control_Surface.begin();
}

void loop() {
    // Update the control surface: read new MIDI events, and read all inputs
    Control_Surface.loop();
}
