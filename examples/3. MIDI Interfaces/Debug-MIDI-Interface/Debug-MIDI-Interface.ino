/** 
 * This example demonstrates how to use the MIDI interface for debugging.
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
 * - Upload the sketch and open the Serial monitor.
 * - Set the line ending to new line or new line and carriage return.
 * - When the button on pin 5 is pressed, a MIDI Note On message is sent for
 *   note C4, and you'll see that event printed in the Serial monitor.
 * - When the button on pin 5 is released, a MIDI Note Off message is sent for 
 *   note C4, and you'll also see that in the monitor.
 * - When you type in "90 3C 7F" (without the quotes), and press enter, the 
 *   built-in LED will light up.
 * - When you type in "80 3C 7F", and press enter, the LED will turn off again.
 * 
 * Mapping
 * -------
 * 
 * None.
 * 
 * Written by PieterP, 2019-08-07  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI Debug interface at 115200 baud.
USBDebugMIDI_Interface midi = 115200;

// Instantiate a NoteButton object
NoteButton button {
  5,                             // Push button on pin 5
  {MIDI_Notes::C[4], Channel_1}, // Note C4 on MIDI channel 1
};

// Instantiate the LED that will light up when middle C is playing
NoteLED led {
  LED_BUILTIN,                   // Pin of built-in LED
  {MIDI_Notes::C[4], Channel_1}, // Note C4 on MIDI channel 1
};

void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface
}
