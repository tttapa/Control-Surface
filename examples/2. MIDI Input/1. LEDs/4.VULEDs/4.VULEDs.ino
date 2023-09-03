/**
 * This example demonstrates the use of the VULEDs class that displays the VU or
 * level meters from your DAW as an LED bar graph.
 * 
 * Shift registers are used to drive the LEDs.
 * 
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Due, Teensy 3.x, ESP8266, ESP32
 * 
 * Connections
 * -----------
 * 
 *   - SS:   74HC595 ST_CP
 *   - MOSI: 74HC595 DS
 *   - SCK:  74HC595 SH_CP
 *   - A0:   Push button (to ground, internal pull-up resistor will be used)
 * 
 * Connect an LED (and series resistor) between each of the first 12 outputs of 
 * the shift registers and ground.
 * 
 * Remember to connect the enable pin of the shift register to ground and the 
 * master reset pin to Vcc in order to enable it.  
 * 
 * Behavior
 * --------
 * 
 * The LEDs display the level of the first track in your DAW.  
 * When the push button is pressed, the VU meter switches from bar mode to dot 
 * mode and back.
 * 
 * Mapping
 * -------
 * 
 * Map the Arduino as a Mackie Control Universal in your DAW.
 * 
 * Written by PieterP, 2019-12-09  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h>

USBMIDI_Interface midi;

// Instantiate two daisy-chained shift register with the SPI slave select pin as
// latch pin, most significant bit first, and a total of 16 outputs.
SPIShiftRegisterOut<16> sreg {SPI, SS, MSBFIRST};

// Instantiate a VULEDs object with 12 LEDs.
MCU::VULEDs<12> vu {
  sreg.pins().slice<0, 11>(), // first 12 pins of the shift registers
  1,                          // track number [1, 8]
  MCU::VUDecay::Default,      // how long does it take for the meter to decay
};

// If you don't want to use shift registers, you can just specify a list of pin
// numbers:
//
//   MCU::VULEDs<12> vu {
//     {{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}}, // Arduino pin numbers
//     1,                      // track number [1, 8]
//     MCU::VUDecay::Default,  // how long does it take for the meter to decay
//   };

// You can also use a different number of LEDs, but keep in mind that the MCU
// protocol sends the level on a scale from 0 to 12.
// If you use 24 LEDs, for example, the LEDs will turn on in groups of 2 at a
// time.

// Push button to switch between dot and bar mode.
Button dotBarModeButton = A0;

void setup() {
  Control_Surface.begin();
  dotBarModeButton.begin();
}

void loop() {
  Control_Surface.loop();
  if (dotBarModeButton.update() == Button::Falling) // when button is pressed
    vu.toggleMode(); // toggle between dot and bar mode
}
