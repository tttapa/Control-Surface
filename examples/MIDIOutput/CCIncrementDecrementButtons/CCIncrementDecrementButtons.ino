/** 
 * This is an example that demonstrates the use of Increment and 
 * Decrement buttons that can be used for scrolling through a song
 * list, tuning effects, etc.
 *
 * ### Connections
 * 
 *   - 5: momentary push button (to ground)
 *   - 6: momentary push button (to ground)
 * 
 * The internal pull-up resistors for the buttons will be enabled automatically.
 * 
 * ### Behavior
 * 
 * If the button on pin 5 is pressed, a MIDI CC increment message is sent, 
 * if the button on pin 6 is pressed, a MIDI CC decrement message is sent.  
 * The controller number `MCU::V_POT_1` will be used.  
 * If either of the buttons is pressed down for a longer time, it will keep on
 * sending increment/decrement messages quickly until you release it. 
 * This is to make it easier to scroll a large amount, for example.  
 * The timing can be changed in Settings/Settings.hpp.  
 * If both buttons are pressed simultaneously, note on/off events is sent on
 * note `MCU::V_POT_SELECT_1`. (This is optional, you can just leave out the
 * final argument to the constructor, and it won't send anything when you press
 * both buttons at once).
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

// Instantiate a CCIncrementDecrementButtons object that reads two buttons,
// connected to pins 5 and 6.
// The CC controller is MCU::V_POT_1, the multiplier is 1, and the reset note
// is MCU::V_POT_SELECT_1
CCIncrementDecrementButtons buttons = {
    {5, 6},
    MCU::V_POT_1,
    1,
    MCU::V_POT_SELECT_1,
};

void setup() {
    // Use the Mackie Control protocol for sending relative MIDI CC messages.
    RelativeCCSender::setMode(MACKIE_CONTROL_RELATIVE);
    Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
    // Update the control surface
    Control_Surface.loop();
}
