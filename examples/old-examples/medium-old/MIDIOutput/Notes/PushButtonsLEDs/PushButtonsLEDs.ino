/**
 * @brief   This example demonstrates the use of push buttons and LEDs.
 * 
 * Connections:
 *  2:  momentary push button (to ground)
 *  3:  momentary push button (to ground)
 *  12: LED (+ series resistor, to ground)
 *  13: LED (+ series resistor, to ground)
 * 
 * The internal pull-up resistors for the buttons will be enabled automatically.
 *  
 * Pressing the first button will mute the first track, and will turn on the 
 * first LED.  
 * Pressing the second button will solo the first track, and will turn on the 
 * second LED.
 * 
 * Map the Arduino as a Mackie Control Universal (MCU) or equivalent in your
 * audio software.
 * 
 * Written by PieterP, 2018-08-28
 * https://github.com/tttapa/Control-Surface
 */

// Include the Control Surface library
#include <Control_Surface.h>

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// Instantiate the momentary push buttons that send out MIDI Note events.
NoteButton buttons[] = {
    {2, MCU::MUTE_1}, // digital input pin, note number, [channel]
    {3, MCU::SOLO_1},
};

// Instantiate the LEDs that will display the state of the incoming MIDI Note
// events.
MIDINoteLED leds[] = {
    {12, MCU::MUTE_1}, // digital output pin, note number, [channel]
    {13, MCU::SOLO_1},
};

void setup() {
    // Initialize everything
    Control_Surface.begin();
}

void loop() {
    // Update the control surface: read new MIDI events, and read all inputs
    Control_Surface.loop();
}