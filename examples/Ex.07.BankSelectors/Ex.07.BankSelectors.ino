/**
 * @example Ex.07.BankSelectors.ino
 * 
 * This is an example of the BankSelector class of the Control_Surface library.
 *
 * Connect two potentiometers to analog pins A0 and A1,
 * and two pushbuttons to pins 2 and 3.
 *
 * Connect push buttons to pins 11 and 12, and 4 LEDs (+ current limiting resitors) to pins 4, 5, 6 and 7.
 *
 * When bank 1 is selected:
 *   - Potentiometer A is channel volume of track 1   &emsp;   (Controller number 0x07, MIDI channel 1)
 *   - Potentiometer B is channel volume of track 2   &emsp;   (Controller number 0x07, MIDI channel 2)
 *   - Mute button A is the mute button for track 1   &emsp;   (Note number 0x10, MIDI channel 1)
 *   - Mute button B is the mute button for track 2   &emsp;   (Note number 0x11, MIDI channel 1)
 *   - The LED on pin 4 lights up.
 *
 * When bank 2 is selected:
 *   - Potentiometer A is channel volume of track 3   &emsp;   (Controller number 0x07, MIDI channel 3)
 *   - Potentiometer B is channel volume of track 4   &emsp;   (Controller number 0x07, MIDI channel 4)
 *   - Mute button A is the mute button for track 3   &emsp;   (Note number 0x12, MIDI channel 1)
 *   - Mute button B is the mute button for track 4   &emsp;   (Note number 0x13, MIDI channel 1)
 *   - The LED on pin 5 lights up.
 *
 * When bank 3 is selected:
 *   - Potentiometer A is channel volume of track 5   &emsp;   (Controller number 0x07, MIDI channel 5)
 *   - Potentiometer B is channel volume of track 6   &emsp;   (Controller number 0x07, MIDI channel 6)
 *   - Mute button A is the mute button for track 5   &emsp;   (Note number 0x14, MIDI channel 1)
 *   - Mute button B is the mute button for track 6   &emsp;   (Note number 0x15, MIDI channel 1)
 *   - The LED on pin 6 lights up.
 *
 * When bank 4 is selected:
 *   - Potentiometer A is channel volume of track 7   &emsp;   (Controller number 0x07, MIDI channel 7)
 *   - Potentiometer B is channel volume of track 8   &emsp;   (Controller number 0x07, MIDI channel 8)
 *   - Mute button A is the mute button for track 7   &emsp;   (Note number 0x16, MIDI channel 1)
 *   - Mute button B is the mute button for track 8   &emsp;   (Note number 0x17, MIDI channel 1)
 *   - The LED on pin 7 lights up.
 *
 * This allows you to control multiple tracks with only a limited amount of physical potentiometers and buttons
 *
 * Map accordingly in your DAW or DJ software.
 *
 * Written by Pieter P, 08-09-2017  
 * https://github.com/tttapa/Control-Surface
*/

#include <Control_Surface.h> // Include the library

// Create a two new instances of the class 'Analog', on pins A0 and A1,
// that send MIDI messages with controller 7 (channel volume) on channels 1 and 2
Analog potentiometer_A(A0, MIDI_CC::Channel_Volume, 1);
Analog potentiometer_B(A1, MIDI_CC::Channel_Volume, 2);

// Create a two new instances of the class 'Digital', on pins 2 and 3,
// that send MIDI messages with note numbers 0x10 and 0x11 on MIDI channel 1
Digital muteButton_A(2, 0x10, 1);
Digital muteButton_B(3, 0x11, 1);

// Create a new bank that has two tracks per bank
Bank bank(2);

// Create a new bank selector that changes the bank setting of the bank we just created
// It has pushbuttons connected to pins 11 and 12 that increment or decrement the bank setting,
// and 4 LEDs to pins 4, 5, 6 and 7 that display the current bank setting.
BankSelector bankSelector(bank, { 11, 12 }, { 4, 5, 6, 7 } );

/*_______________________________________________________________________________________________________________________________________*/

void setup() {
  // Add the created objects to the bank
  bank.add(potentiometer_A, Bank::CHANGE_CHANNEL); // When the bank setting is changed, change the channel of the potentiometer
  bank.add(potentiometer_B, Bank::CHANGE_CHANNEL);
  bank.add(muteButton_A, Bank::CHANGE_ADDRESS); // When the bank setting is changed, change the address (note number) of the mute button
  bank.add(muteButton_B, Bank::CHANGE_ADDRESS);
}

/*_______________________________________________________________________________________________________________________________________*/

void loop() {
  // Refresh the MIDI controller (check whether the inputs have changed since last time, if so, send the new value over MIDI)
  // It also refreshes the bank selector
  Control_Surface.refresh();
}