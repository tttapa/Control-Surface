/**
 * @example Ex.06.Banks.ino
 * 
 * This is an example of the Bank class of the Control_Surface library.
 * 
 * Connect two potentiometers to analog pins A0 and A1,
 * and two pushbuttons to pins 2 and 3.
 *
 * Connect a toggle switch to pin 11.
 *
 * When the toggle switch is turned off:
 *  - Potentiometer A is channel volume of track 1    (Controller number 0x07, MIDI channel 1)
 *  - Potentiometer B is channel volume of track 2    (Controller number 0x07, MIDI channel 2)
 *  - Mute button A is the mute button for track 1      (Note number 0x10, MIDI channel 1)
 *  - Mute button B is the mute button for track 2      (Note number 0x11, MIDI channel 1)
 *
 * When the toggle switch is turned on:
 *  - Potentiometer A is channel volume of track 3    (Controller number 0x07, MIDI channel 3)
 *  - Potentiometer B is channel volume of track 4    (Controller number 0x07, MIDI channel 4)
 *  - Mute button A is the mute button for track 3      (Note number 0x12, MIDI channel 1)
 *  - Mute button B is the mute button for track 4      (Note number 0x13, MIDI channel 1)
 *
 * This allows you to control multiple tracks with only a limited amount of physical potentiometers and buttons
 *
 * Map accordingly in your DAW or DJ software.
 *
 * Note: It is recommended to use a BankSelector (See Ex.07.BankSelectors.ino) instead of changing the bank setting manually.
 *
 * ### How it works:
   @par
   When the bank setting is set to 1,
    - 2 (i.e. the number of tracks per bank) will be added to the channel number of the potentiometers.  
      This is because they were added to the bank using the CHANGE_CHANNEL mode.
    - 2 (i.e. the number of tracks per bank) will be added to the note numbers of the buttons.  
      This is because they were added to the bank using the CHANGE_ADDRESS mode;

  
  ### In general:  
  @par
    For controls that were added using the CHANGE_ADDRESS mode:
      - The address will be: startAddress + tracksPerBank * bankSetting
      - The channel won't change.
  @par
    For controls that were added using the CHANGE_CHANNEL mode:
      - The channel will be: startChannel + tracksPerBank * bankSetting
      - The address won't change.

 * Written by Pieter P, 08-09-2017  
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

void setup() {
  // Add the created objects to the bank
  bank.add(potentiometer_A, Bank::CHANGE_CHANNEL); // When the bank setting is changed, change the channel of the potentiometer
  bank.add(potentiometer_B, Bank::CHANGE_CHANNEL);
  bank.add(muteButton_A, Bank::CHANGE_ADDRESS); // When the bank setting is changed, change the address (note number) of the mute button
  bank.add(muteButton_B, Bank::CHANGE_ADDRESS);

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