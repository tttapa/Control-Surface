/** 
 * This is an example that demonstrates the use of MIDI Note Buttons that can be
 * used for triggering samples, mute/solo/rec buttons, play/pause/stop buttons, 
 * etc. It can control almost any push button control in your DAW software.
 *
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Teensy 3.x, ESP32, ESP8266
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
 * Note numbers use the [Scientific Pitch Notation system](https://en.wikipedia.org/wiki/Scientific_pitch_notation),
 * where <b>A<sub>4</sub></b> is 440 Hz, and <b>C<sub>-1</sub></b> is 8.1758 Hz.
 * See @ref MIDI_Notes for a table of MIDI note numbers.
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

#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// Instantiate a NoteButton object
NoteButton button {
  5,                             // Push button on pin 5
  {MIDI_Notes::C[4], Channel_1}, // Note C4 on MIDI channel 1
};

void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface
}