/**
 * @brief   This is an example of the `BluetoothMIDI_Interface` class of the 
 *          Control_Surface library. (ESP32 only)
 * 
 * Connect a momentary push button to pin 18.  
 * Pressing it will send middle C MIDI Note events.
 * 
 * On Android, I used the MIDI BLE Connect and MIDI Scope apps by Mobileer Inc
 * to test it.
 *  
 * Written by Pieter P, 26-06-2019  
 * https://github.com/tttapa/Control-Surface
 */

// Include the Control Surface library and the Bluetooth MIDI Interface
#include <Control_Surface.h>
#include <MIDI_Interfaces/BluetoothMIDI_Interface.hpp>

// Instantiate a MIDI over BLE interface.
BluetoothMIDI_Interface midi;

using namespace MIDI_Notes;

// Create a new instance of the class `NoteButton`, called `button`,
// on pin 18, that sends MIDI Note messages with middle C and maximum velocity
// on channel 1.
NoteButton button = {18, {note(C, 4), CHANNEL_1}};

void setup() {
    // Initialize everything
    Control_Surface.begin();
}

void loop() {
    // Update the Control Surface (check whether the potentiometer's input has 
    // changed since last time, if so, send the new value over BLE MIDI).
    Control_Surface.loop();
}