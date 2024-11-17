/**
 * @brief   This is an example of the ProgramChanger and Selector classes.
 * 
 * ### Connections
 *  - 5: A momentary push button (to ground)
 *  - 6: A momentary push button (to ground)
 *  
 * The internal pull-up resistors will be enabled automatically
 *  
 * ### Behavior
 * When you press the first button, the next program is selected.  
 * When you press the second button, the previous program is selected.  
 * When you press both buttons at once, it resets to the first program
 * again.  
 * If you keep pressin one of the buttons, it will cycle through the
 * programs more quickly.
 * 
 * Written by Pieter P, 30-01-2019  
 * https://github.com/tttapa/Control-Surface
 */

#include <Encoder.h> // Needed for the EncoderSelector to work
// Encoder must be included before Control Surface
#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI interface to use for the Control Surface.
USBDebugMIDI_Interface midi;

// Instantiate a program changer with four programs: 0x10, 0x20, 0x30, 0x40
// on channel 1
ProgramChanger<4> pc = {
    {0x10, 0x20, 0x30, 0x40},
    CHANNEL_1,
};

// Instantiate a selector for the program changer on pins 5 (increment) and
// pin 6 (decrement). 
// It doesn't wrap around when the maximum/minimum is reached.
IncrementDecrementSelector<4> sel = {
  pc, 
  {5, 6},
  Wrap::NoWrap,
};

// You could try other selectors as well, for example
//
// EncoderSelector<4> sel = {
//   pc,
//   {2, 3}, // rotary encoder connected between these pins and ground
//   4, // change one setting for each 'click' (= 4 pulses)
//   Wrap::Wrap,
// };
//
// ManyButtonsSelector<4> sel = {
//   pc,
//   {2, 3, 5, 7}, // push buttons connected between these pins and ground
// };

void setup() {
    Control_Surface.begin(); // Initialize Control Surface
}

void loop() { 
  Control_Surface.loop(); // Update the Control Surface
}