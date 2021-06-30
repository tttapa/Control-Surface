/**
 * This example demonstrates the use of LEDs that respond to incoming  MIDI
 * note events. The LEDs are driven by a 74HC595 (or equivalent) shift register.
 * 
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 *   - SS:   74HC595 ST_CP
 *   - MOSI: 74HC595 DS
 *   - SCK:  74HC595 SH_CP
 * 
 * Connect eight LEDs (and series resistors) between the outputs of the shift
 * register and ground.
 * 
 * Remember to connect the enable pin of the shift register to ground and the 
 * master reset pin to Vcc in order to enable it.  
 * 
 * Behavior
 * --------
 * 
 * If a MIDI Note On event for note 0x3C (C4 or middle C) is sent, the first
 * LED will light up, if a Note Off event for that note is sent, the LED will 
 * turn off.  
 * If a MIDI Note On event for note 0x3D (C#4) is sent, the second LED will
 * light up, and so on.  
 * (A Note On event with a velocity of zero also counts as a Note Off event.)
 * 
 * Mapping
 * -------
 * 
 * Route the MIDI output of a MIDI keyboard to the Arduino's MIDI input. Then
 * play some notes between middle C and middle G on the keyboard. The LEDs 
 * should light up when you press the keys.
 * 
 * Written by PieterP, 2019-10-09  
 * https://github.com/tttapa/Control-Surface
 */

// Include the library
#include <Control_Surface.h>

// Instantiate a MIDI Interface to use
USBMIDI_Interface midi;

// Instantiate a shift register as output for the LEDs
SPIShiftRegisterOut<8> sreg {
  SPI,      // SPI interface to use
  10,       // Latch pin (ST_CP)
  MSBFIRST, // Byte order
};

// Create a range of LEDs that listens for MIDI Note messages, turning on and off
// the LEDs connected to the eight output pins of the shift register
NoteRangeLEDs<8> leds {sreg.pins(), MIDI_Notes::C(4)};

// Initialize the Control Surface
void setup() {
  Control_Surface.begin();
}

// Update the Control Surface
void loop() {
  Control_Surface.loop();
}