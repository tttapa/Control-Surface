/**
 * @example Ex.17.Transposer.ino
 * 
 * @brief An example demonstrating the transpose function.
 * 
 * Connect 8 buttons to pins 2 - 9, to use as piano keys 
 * and two buttons to pins 10 - 11 to transpose the notes.
 * 
 * Pressing button 10 once raises the pitch by one semitone,
 * if you press it again, the pitch is two semitones higher, etc.
 * Pressing button 11 lowers the pitch by one semitone at a time.
 * 
 * Written by Pieter P, 27-05-2018   
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h>

Digital buttons[] = {
  {2, 0x3C, 1}, // pin 2, note C4, channel 1
  {3, 0x3E, 1}, // pin 3, node D4, channel 1
  {4, 0x40, 1}, // pin 4, note E4, channel 1
  {5, 0x41, 1}, // pin 5, node F4, channel 1
  {6, 0x43, 1}, // pin 6, note G4, channel 1
  {7, 0x45, 1}, // pin 7, node A4, channel 1
  {8, 0x47, 1}, // pin 8, note B4, channel 1
  {9, 0x48, 1}, // pin 9, node C5, channel 1
};

Bank transposedButtons; // Create a bank (i.e. collection of buttons and other inputs)
Transposer transposer(transposedButtons, {10, 11}, -12, +12); // Create a transposer that works on the bank, 
                                                              // with an increment button on pin 10 and a
                                                              // decrement button on pin 11, 
                                                              // with a minimum of -12 semitones (i.e.
                                                              // one octave lower) and a maximum of +12
                                                              // semitones (i.e. one octave higher).

void setup() {
  transposedButtons.add(buttons); // Add the buttons to the bank
}

void loop() {
  Control_Surface.refresh(); // refresh everything
}