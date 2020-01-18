/** 
 * This example demonstrates how to use a Serial MIDI Interface, for use with
 * 5-pin DIN connectors, for example.
 *
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33, Teensy 3.x, ESP32
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
 * - This is just a simple MIDI controller with a single note button and an LED.
 * 
 * Mapping
 * -------
 * 
 * None.
 * 
 * Written by PieterP, 2020-01-09  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

// Select the serial port to use.
auto &serial = Serial;
// Instantiate a Serial MIDI interface at the default MIDI baud rate.
SerialMIDI_Interface<decltype(serial)> midi = {serial, MIDI_BAUD};

// You can also use the following serial MIDI interfaces:
// 
// The Serial port that is connected to your computer over USB:
//
//    USBSerialMIDI_Interface midi = 115200;
//
// A hardware serial port:
// 
//    HardwareSerialMIDI_Interface midi = {Serial1, MIDI_BAUD};

using namespace MIDI_Notes;

// Instantiate a NoteButton object
NoteButton button = {
  5,                       // Push button on pin 5
  {note(C, 4), CHANNEL_1}, // Note C4 on MIDI channel 1
};

// Instantiate the LED that will light up when middle C is playing
NoteValueLED led = {
  LED_BUILTIN,             // Pin of built-in LED
  {note(C, 4), CHANNEL_1}, // Note C4 on MIDI channel 1
};

void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface
}
