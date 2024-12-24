/**
 * @brief   This example demonstrates the use of push buttons that send note 
 *          events.
 * 
 * ### Connections
 * 
 *  - 2:  momentary push button (to ground)
 *  - 3:  momentary push button (to ground)
 * 
 * The internal pull-up resistors for the buttons will be enabled automatically.
 * 
 * ### Behavior
 * 
 * Pressing the first button will send note on/off events on middle C.
 * Pressing the second button will send note on/off events for middle C sharp.
 * 
 * Written by PieterP, 28-01-2019  
 * https://github.com/tttapa/Control-Surface
 */

// Include the Control Surface library
#include <Control_Surface.h>

// Instantiate a MIDI over USB interface.
USBDebugMIDI_Interface midi;

using namespace MIDI_Notes;

// Instantiate the momentary push buttons that send out MIDI Note events.
NoteButton buttons[] = {
    {2, note(C, 4)}, // digital input pin, note number
    {3, note(Db, 4)},
    
    // If you want to set the channel specifically, you can use:
    // {2, {note(C, 4), CHANNEL_1}},
};

void setup() {
    // Initialize everything
    Control_Surface.begin();
}

void loop() {
    // Update the control surface
    Control_Surface.loop();
}