/**
 * @example Ex.13.Shift-Register.ino
 * 
 * This is an example of the ShiftRegisterOut class of the Control_Surface library.
 * 
 * Connect a potentiometer to analog pins A0.  
 * Connect push buttons to pins 2 and 3, and a shift register to pins 10 (ST_CP), 11 (DS) and 13 (SH_CP).  
 * Connect LEDs (+ current limiting resistors) to the 8 outputs of the shift register.
 *
 * Pins 2 and 3 increment or decrement the bank setting, and the LEDs display the bank setting.
 * 
 * This allows you to control multiple tracks with only a limited amount of physical potentiometers and buttons.
 * 
 * Depending on the bank setting, the potentiometer will send MIDI Control Change 
 * as the channel volume of channels 1 through 8.  
 * Map accordingly in your DAW or DJ software.
 *
 * Written by Pieter P, 08-09-2017  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the library

const pin_t dataPin  = 11; //Pin connected to DS of 74HC595
const pin_t clockPin = 13; //Pin connected to SH_CP of 74HC595
const pin_t latchPin = 10; //Pin connected to ST_CP of 74HC595

// Create a new shift register output connected to pins 11, 13 and 10, shift the data out with the most significant bit first, 
// There are 8 outputs in total
ShiftRegisterOut ShiftReg(dataPin, clockPin, latchPin, MSBFIRST, 8);

// Create a new instance of the class `CCPotentiometer`, on pin A0,
// that sends MIDI messages with controller 7 (channel volume) on MIDI channel 1
CCPotentiometer potentiometer(A0, MIDI_CC::Channel_Volume, 1);

// Create a new bank that has one track per bank
Bank bank(1);

// Create a new bank selector that changes the bank setting of the bank we just created
// It has pushbuttons connected to pins 2 and 3 that increment or decrement the bank setting,
// and 8 LEDs connected to the shift register, that display the current bank setting.
BankSelector bankSelector(bank, { 2, 3 }, { 
  ShiftReg.pin(0), 
  ShiftReg.pin(1), 
  ShiftReg.pin(2), 
  ShiftReg.pin(3), 
  ShiftReg.pin(4), 
  ShiftReg.pin(5), 
  ShiftReg.pin(6), 
  ShiftReg.pin(7)
});

/*_______________________________________________________________________________________________________________________________________*/

void setup() {
  // Add the potentiometer control object to the bank
  bank.add(potentiometer, Bank::CHANGE_CHANNEL); // When the bank setting is changed, change the channel of the potentiometer
}

/*_______________________________________________________________________________________________________________________________________*/

void loop() {
  // Update the MIDI controller (check whether the inputs have changed since last time, if so, send the new value over MIDI)
  // It also refreshes the bank selector
  Control_Surface.update();
}
