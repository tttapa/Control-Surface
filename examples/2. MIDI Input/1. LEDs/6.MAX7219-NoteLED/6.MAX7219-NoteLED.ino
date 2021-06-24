/**
 * This example demonstrates the use of LEDs that respond to incoming  MIDI
 * note events. The LEDs are driven by a MAX7219.
 * 
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 *   - SS:   MAX7219 CS
 *   - MOSI: MAX7219 DIN
 *   - SCK:  MAX7219 CLK 
 * 
 * Behavior
 * --------
 * 
 * If a MIDI Note On event for note 0x3C (C4 or middle C) is sent, the first 
 * LED of the MAX7219 will light up, if a Note Off event for that note is sent, 
 * the LED will turn off.  
 * (A Note On event with a velocity of zero also counts as a Note Off event.)
 * 
 * Mapping
 * -------
 * 
 * Route the MIDI output of a MIDI keyboard to the Arduino's MIDI input. Then
 * play a middle C on the keyboard.
 * 
 * Written by PieterP, 2020-03-24  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// Instantiate a MAX7219 with the SPI slave select pin as latch pin
// There's just 1 MAX7219 in the chain, if you have more of them daisy-chained
// together, you can increase the template argument (between angled brackets)
MAX7219<1> max7219 {SPI, SS};

// Instantiate the LED that will light up when middle C is playing
NoteLED led {
  max7219.pin(0),              // First pin of the MAX7219
  {MIDI_Notes::C(4), CHANNEL_1}, // Note C4 on MIDI channel 1
};

void setup() {
  Control_Surface.begin(); // Initialize Control Surface
  // No need to initialize the MAX7219 or to set pinModes, all of this
  // is handled by Control Surface.
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface
}
