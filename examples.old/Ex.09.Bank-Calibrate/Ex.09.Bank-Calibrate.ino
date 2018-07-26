/**
 * @example Ex.09.Bank-Calibrate.ino
 * 
 * This is an example of the Bank class of the Control_Surface library.
 * 
 * Connect two potentiometers to analog pins A0 and A1,
 * and two pushbuttons to pins 2 and 3.
 * 
 * Connect a toggle switch to pin 11.
 * 
 * When the toggle switch is turned off:
 *   - Potentiometer A is channel volume of track 1   &emsp;   (Controller number 0x07, MIDI channel 1)
 *   - Potentiometer B is channel volume of track 2   &emsp;   (Controller number 0x07, MIDI channel 2)
 *   - Mute button A is the mute button for track 1   &emsp;   (Note number 0x10, MIDI channel 1)
 *   - Mute button B is the mute button for track 2   &emsp;   (Note number 0x11, MIDI channel 1)
 * 
 * When the toggle switch is turned on:
 *   - Potentiometer A is channel volume of track 3   &emsp;   (Controller number 0x07, MIDI channel 3)
 *   - Potentiometer B is channel volume of track 4   &emsp;   (Controller number 0x07, MIDI channel 4)
 *   - Mute button A is the mute button for track 3   &emsp;   (Note number 0x12, MIDI channel 1)
 *   - Mute button B is the mute button for track 4   &emsp;   (Note number 0x13, MIDI channel 1)
 * 
 * This allows you to control multiple tracks with only a limited amount of physical potentiometers and buttons
 * 
 * Map accordingly in your DAW or DJ software.
 * 
 * Set 'minimumValue' to the analog value read when the potentiometer is in the minimum position.  
 * Set 'maximumValue' to the analog value read when the potentiometer is in the maximum position.
 *
 * Note: It is recommended to use a BankSelector (See Ex.07.BankSelectors) instead of changing the bank setting manually.
 *
 * Written by Pieter P, 20-09-2017  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the library

const pin_t bankSwitchPin = 11; // The pin with the bank selection switch connected

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

/*_______________________________________________________________________________________________________________________________________*/

const int minimumValue = 3;    // the analog value read when the potentiometer is in the minimum position.
const int maximumValue = 1020; // the analog value read when the potentiometer is in the maximum position.

int mapCalibrated(int value) {
  value = constrain(value, minimumValue, maximumValue); // make sure that the analog value is between the minimum and maximum value
  return map(value, minimumValue, maximumValue, 0, 1023); // map the value from [minimumValue, maximumValue] to [0, 1023]
}

/*_______________________________________________________________________________________________________________________________________*/

void setup() {
  // Add the created objects to the bank
  bank.add(potentiometer_A, Bank::CHANGE_CHANNEL); // When the bank setting is changed, change the channel of the potentiometer
  bank.add(potentiometer_B, Bank::CHANGE_CHANNEL);
  bank.add(muteButton_A, Bank::CHANGE_ADDRESS); // When the bank setting is changed, change the address (note number) of the mute button
  bank.add(muteButton_B, Bank::CHANGE_ADDRESS);

  bank.map(mapCalibrated); // apply the 'mapCalibrated' function on the analog input value of all Analog objects in the bank

  pinMode(bankSwitchPin, INPUT_PULLUP);
}

/*_______________________________________________________________________________________________________________________________________*/

void loop() {
  if (digitalRead(bankSwitchPin) == LOW) // if the bank switch is switched on
    bank.select(1); // select tracks 3 and 4
  else
    bank.select(0); // select tracks 1 and 2

  // Refresh the MIDI controller (check whether the inputs have changed since last time, if so, send the new value over MIDI)
  Control_Surface.refresh();
}