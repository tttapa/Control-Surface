/**
 * @brief   This is the code I used for my specific MIDI controller.
 * 
 * I used an Arduino Leonardo with 4 faders on A0-A3, 8 potentiometers on 
 * A4-A11,a rotary encoder on pins 0 & 1, 4 toggle switches on pins 
 * 2, 3, 5 and 7, a toggle switch (for bank select) on pin 11, 
 * and an LED on pin 13.
 *
 * When bank select is 0, faders 1-4 are channel volumes 1-4, potSide 1-4 are 
 * pan/balance of channel 1-4, switches 1-4 are mute channel 5-8.  
 * When bank select is 1, faders 1-4 are channel volumes 5-8, potSide 1-4 are 
 * pan/balance of channel 5-8, switches 1-4 are mute channel 1-4.  
 * PotTop 1-4 are not in the bank, because I use them as effect or 
 * EQ parameters, independent of the track.  
 * Enc1 is used for scrolling.
 *
 * Note that I used a custom control mapping in my DAW.
 *
 * Written by tttapa, 07-09-2017
 * https://github.com/tttapa/Control-Surface
 */

#include <Encoder.h> // Include the Encoder library
// This has to be done before including the Control Surface library
#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI over USB interface
USBMIDI_Interface usbmidi;

// Create a new Bank with two bank settings, and four tracks per bank.
// This allows for a total of eight tracks.
Bank<2> bank(4);

// Create a selector to select the bank setting.
// It's a simple switch connected between pin 11 and ground.
// The state of the switch is displayed using an LED connected to pin 13.
// When it is switched off, the bank setting is 0, and when it is switched on,
// the bank setting is 1.
SwitchSelectorLEDs selector = {bank, 13, 11};

// If the jog wheels or other encoders are too slow in your software, 
// increase this value. (It will be multiplied with the actual speed of the 
// encoder, as the name implies.) Default is 1.
const int speedMultiply = 1; 

// Create four new instances of the Bankable CCPotentiometer class.
// These are connected to my four faders on pins A0-A3.
// They send out MIDI Channel Volume Control Change events on channels 1-4.
Bankable::CCPotentiometer faders[] = {
    {{bank, CHANGE_CHANNEL}, A0, {MIDI_CC::Channel_Volume, CHANNEL_1}},
    {{bank, CHANGE_CHANNEL}, A1, {MIDI_CC::Channel_Volume, CHANNEL_2}},
    {{bank, CHANGE_CHANNEL}, A2, {MIDI_CC::Channel_Volume, CHANNEL_3}},
    {{bank, CHANGE_CHANNEL}, A3, {MIDI_CC::Channel_Volume, CHANNEL_4}},
};

CCPotentiometer knobsTop[] = {
    {A4, 0x10},
    {A5, 0x11},
    {A6, 0x12},
    {A7, 0x13},
};

Bankable::CCPotentiometer knobsSide[] = {
    {{bank, CHANGE_CHANNEL}, A8,  {MIDI_CC::Pan, CHANNEL_1}},
    {{bank, CHANGE_CHANNEL}, A9,  {MIDI_CC::Pan, CHANNEL_2}},
    {{bank, CHANGE_CHANNEL}, A10, {MIDI_CC::Pan, CHANNEL_3}},
    {{bank, CHANGE_CHANNEL}, A11, {MIDI_CC::Pan, CHANNEL_4}},
};

Bankable::NoteButtonLatching switches[] = {
    {bank, 2, 0x10},
    {bank, 3, 0x11},
    {bank, 5, 0x12},
    {bank, 7, 0x13},
};

CCRotaryEncoder enc = {{1, 0}, 0x2F, speedMultiply, 4};

void setup() {
    Control_Surface.begin();
}

void loop() { // Refresh all inputs
    Control_Surface.loop();
}
