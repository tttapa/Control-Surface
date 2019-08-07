/**
 * @example   "NoteButton.ino"
 * 
 * NoteButton
 * ==========
 * 
 * This is an example that demonstrates the use of MIDI Note Buttons that can be
 * used for triggering samples, mute/solo/rec buttons, play/pause/stop buttons, 
 * etc. It can control almost any push button control in your DAW software.
 *
 * Connections
 * -----------
 * 
 * - 5: momentary push button (to ground)
 * 
 * The internal pull-up resistor for the button will be enabled automatically.
 * 
 * Behavior
 * --------
 * 
 * - When the button on pin 5 is pressed, a MIDI Note On message is sent for
 *   note C4.
 * - When the button on pin 5 is released, a MIDI Note Off message is sent for 
 *   note C4.
 * 
 * Mapping
 * -------
 * 
 * Select the Arduino as a custom MIDI controller in your DAW, and use the 
 * MIDI learn option to assign the button to a function.
 * 
 * Written by PieterP, 2019-08-07  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "CCIncrementDecrementButtons.ino"
 * 
 * CCIncrementDecrementButtons
 * ===========================
 * 
 * This is an example that demonstrates the use of Increment and 
 * Decrement buttons that can be used for scrolling through a song
 * list, tuning effects, etc.  
 * You can compare it to a rotary encoder, but with two buttons instead of an 
 * encoder.
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
 *   The timing can be changed by tweaking @ref LONG_PRESS_DELAY and 
 *   @ref LONG_PRESS_REPEAT_DELAY in Settings/Settings.hpp.
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

/**
 * @example   "MIDI-Input-Callback.ino"
 * 
 * MIDI-Input-Callback
 * ===================
 * 
 * This is an example on how to attach your own callbacks for receiving MIDI 
 * input data.
 *
 * Connections
 * -----------
 * 
 * None.
 * 
 * Behavior
 * --------
 * 
 * - When MIDI messages are sent to the USB interface, they are printed to the 
 *   Serial port.
 * 
 * Mapping
 * -------
 * 
 * None.
 * 
 * Written by PieterP, 2019-08-07  
 * https://github.com/tttapa/Control-Surface
 */

