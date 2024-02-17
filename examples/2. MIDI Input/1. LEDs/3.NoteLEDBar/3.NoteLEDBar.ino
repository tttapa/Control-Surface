/**
 * This example demonstrates the use of LED bar graphs that respond to incoming 
 * MIDI note events. The LEDs are driven by a 74HC595 (or equivalent) shift register.
 * 
 * @boards  AVR, AVR USB, Nano Every, Teensy 3.x, ESP32
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
 * If a MIDI Note On event for note 0x3C (C4 or middle C) is sent, the LED bar
 * display is turned on, the higher the velocity (the harder you press the key),
 * the more LEDs will turn on.
 * 
 * Mapping
 * -------
 * 
 * Route the MIDI output of a MIDI keyboard or from an audio workstation to 
 * the Arduino's MIDI input.
 * 
 * Written by PieterP, 2019-11-25  
 * https://github.com/tttapa/Control-Surface
 */

// Include the library
#include <Control_Surface.h>

// Instantiate a MIDI Interface to use
USBDebugMIDI_Interface midi;

// Instantiate a shift register as output for the LEDs
SPIShiftRegisterOut<8> sreg {
  SPI,      // SPI interface to use
  10,       // Latch pin (ST_CP)
  MSBFIRST, // Byte order
};

// Create a LED bar driver that listens for MIDI Note C4 that drives
// the LEDs connected to the eight output pins of the shift register
NoteLEDBar<8> leds {sreg.pins(), MIDI_Notes::C[4]};

// Initialize the Control Surface
void setup() {
  Control_Surface.begin();
}

// Update the Control Surface
void loop() {
  Control_Surface.loop();
}

/*
 * Extra
 * =====
 * 
 * 1. The exact same approach can be used to make a LED bar graph that shows
 *    the value of a Control Change input.
 *    In that case, use CCLEDBar instead of NoteLEDBar.
 *    
 * 2. You can use any type of pins for the LEDs, and in any combination.
 *    For example, if you want to use the normal Arduino pins 2-9:
 *    
 *      NoteLEDBar<8> leds { 
 *          {{2, 3, 4, 5, 6, 7, 8, 9}}, 
 *          MIDI_Notes::C[4], 
 *      };
 *    
 *    Note the use of double braces for the list of numbers.
 *    
 *    You can even mix and match pins of different types:
 *    
 *      NoteLEDBar<8> leds { 
 *          {{2, 3, 4, 5, 6, 7, sreg.pin(0), sreg.pin(1) }},
 *          MIDI_Notes::C[4], 
 *      };
 */
