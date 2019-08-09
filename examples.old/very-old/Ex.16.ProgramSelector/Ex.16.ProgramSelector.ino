/**
 * @example Ex.16.ProgramSelector.ino
 * 
 * This is an example of the ProgramSelector class of the Control_Surface library.
 * 
 * Connect two push buttons between pins 11 and 12 and ground,
 * pull-up resistors are not necessary, because the internal ones will be used.
 * 
 * When the buttons are pressed, the patch number changes (see Selector for 
 * more information and the use of different selecting modes), and a 
 * MIDI Program Change event is sent.
 * 
 * Written by Pieter P, 11-04-2018   
 * https://github.com/tttapa/Control-Surface
*/

#include <Control_Surface.h> // Include the library

const uint8_t channel = 1;
// Patch Selector selecting patches 1, 2, 3 or 4 on MIDI channel 1,
// increment patch number with button connected to pin 11, 
// decrement patch number with button connected to pin 12
ProgramSelector sel( { 1, 2, 3, 4 }, channel, { 11, 12 } ); 

void setup() {} // Nothing to set up

void loop() {
  // Update the control surface, send a Program Change 
  // event when the patch number changes
  Control_Surface.update(); 
}