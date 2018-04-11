/**
 * @example Ex.16.SelectorPC.ino
 * 
 * This is an example of the SelectorPC class of the Control_Surface library.
 * 
 * Connect two push buttons between pins 11 and 12 and ground,
 * pull-up resistors are not necessary, because the internal ones will be used.
 * 
 * When the buttons are pressed, the patch number changes (see Selector for 
 * more information and the use of different selecting modes), and a 
 * MIDI Program Change event is sent.
 * 
 * Written by Pieter P, 11-04-2018   
 * https://github.com/tttapa/Control_Surface
*/

#include <Control_Surface.h> // Include the library

// Patch Selector selecting patches 1, 2, 3 or 4, 
// increment patch number with button connected to pin 11, 
// decrement patch number with button connected to pin 12,
// 4 patches in total
SelectorPC sel( { 1, 2, 3, 4 }, { 11, 12 }, 4 ); 


void setup() {} // Nothing to set up

void loop() {
  // Refresh the control surface, send a Program Change 
  // event when the patch number changes
  Control_Surface.refresh(); 
}
