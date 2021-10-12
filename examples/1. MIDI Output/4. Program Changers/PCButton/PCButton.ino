/** 
 * This is an example that demonstrates the use of MIDI Program Change buttons.
 *
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 * - 2: momentary push button (to ground)
 * 
 * The internal pull-up resistor for the button will be enabled automatically.
 * 
 * Behavior
 * --------
 * 
 * - When the button on pin 2 is pressed, the MIDI Program for 
 *   "Steel Drums" is selected.
 * 
 * Written by PieterP, 2019-08-07  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI over USB interface
USBMIDI_Interface midi;

// Instantiate a PCButton that reads the input from a push button and sends out
// a MIDI Program Change message when it's pressed.
PCButton pcBtn {
  2,                                 // pin
  {MIDI_PC::Steel_Drums, CHANNEL_1}, // address
};

void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface
}
