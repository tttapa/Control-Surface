/** 
 * This is an example that demonstrates the use of Increment and 
 * Decrement buttons that can be used for scrolling through a song
 * list, tuning effects, etc.  
 * You can compare it to a rotary encoder, but with two buttons instead of an 
 * encoder.
 *
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, Teensy 3.x, ESP32
 *
 * Connections
 * -----------
 * 
 * - 5: momentary push button (to ground)
 * - 6: momentary push button (to ground)
 * 
 * The internal pull-up resistors for the buttons will be enabled automatically.
 * 
 * Behavior
 * --------
 * 
 * - If the button on pin 5 is pressed, a MIDI CC increment message is sent.
 * - If the button on pin 6 is pressed, a MIDI CC decrement message is sent.
 * - The controller number `MCU::V_POT_1` will be used.
 * - If either of the buttons is pressed down for a longer time, it will keep on
 *   sending increment/decrement messages quickly until you release it (like a 
 *   key on a computer keyboard).
 *   This is to make it easier to scroll a large amount, for example.
 *   The timing can be changed by tweaking @ref AH::LONG_PRESS_DELAY and 
 *   @ref AH::LONG_PRESS_REPEAT_DELAY in @ref AH/Settings/Settings.hpp.
 * - If both buttons are pressed simultaneously, a MIDI Note On event is sent on
 *   note `MCU::V_POT_SELECT_1`.  
 *   (This is optional, you can just leave out the final argument to the 
 *   constructor, and it won't send anything when you press both buttons at 
 *   once).
 * - The multiplier can be used to increment multiple ticks per each time you 
 *   press a button. This is useful if you have to press the button for a very
 *   long time to get a substantial change.
 * 
 * Map the Arduino as a Mackie Control Universal (MCU) or equivalent in your
 * audio software.
 * 
 * Written by PieterP, 2019-01-26  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// Instantiate a CCIncrementDecrementButtons object
CCIncrementDecrementButtons buttons {
  {5, 6},              // Button pins: 5 increments, 6 decrements
  MCU::V_POT_1,        // Increment/Decrement CC address
  1,                   // Multiplier
  MCU::V_POT_SELECT_1, // Reset note address
};

void setup() {
  // Use the Mackie Control protocol for sending relative MIDI CC messages.
  RelativeCCSender::setMode(MACKIE_CONTROL_RELATIVE);

  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Update the control surface
}
