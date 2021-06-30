/**
 * This is an example of the ShiftRegisterOut class of the Control Surface 
 * library. It creates light effects using three 8-bit shift registers and some
 * RGB LEDs.
 *
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, Due, Teensy 3.x, ESP8266, ESP32
 *
 * Connections
 * -----------
 *
 * Connect three daisy-chained shift registers to the SPI pins.  
 * Connect 8 RGB LEDs (+ current limiting resistors) to the outputs of the shift
 * registers.
 * 
 * ```
 * SCK   >───────────┬──────────────────────┬──────────────────────┐
 *           ┏━━━━━━━┷━━━━━━━┓      ┏━━━━━━━┷━━━━━━━┓      ┏━━━━━━━┷━━━━━━━┓
 *           ┃     SH_CP     ┃      ┃     SH_CP     ┃      ┃     SH_CP     ┃
 * MOSI  >───┨ DS        Q7S ┠──────┨ DS        Q7S ┠──────┨ DS        Q7S ┃
 *           ┃     ST_CP     ┃      ┃     ST_CP     ┃      ┃     ST_CP     ┃
 *           ┗━━━━━━━┯━━━━━━━┛      ┗━━━━━━━┯━━━━━━━┛      ┗━━━━━━━┯━━━━━━━┛
 * SS    >───────────┴──────────────────────┴──────────────────────┘
 * ```
 * Remember to connect the enable pin of the shift register to ground and the 
 * master reset pin to Vcc in order to enable it.  
 * 
 * The order of the colors doesn't matter. You can change them in software using
 * the `ShiftRegisterOutRGB::redBit`, `ShiftRegisterOutRGB::greenBit` and 
 * `ShiftRegisterOutRGB::blueBit` constants.
 * 
 * If you wired the LEDs as RGB (red first, then green and then blue), the red 
 * bit is 0, the green bit is 1 and the blue bit is 2.  
 * If you wired the LEDs as BGR (blue first, then green and then red), the red
 * bit is 2, the green bit is 1 and the blue bit is 0.  
 * Other combinations are possible as well.
 * 
 * Behavior
 * --------
 * 
 * This example will turn on all red LEDs one by one, then turn them off one by
 * one, next, it will turn on and off all green LEDs in the same manner, and
 * finally the same for the blue LEDs. This is repeated indefinitely.
 * 
 * Demo
 * ----
 * 
 * @htmlonly
 * <iframe width="560" height="315"
 * src="https://www.youtube.com/embed/7ywcTTdnseU?loop=1" frameborder="0"
 * allow="accelerometer; autoplay; encrypted-media; gyroscope;
 * picture-in-picture" allowfullscreen></iframe>
 * @endhtmlonly
 * 
 * Written by Pieter P, 2018-07-13  
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Arduino_Helpers.h> // Include the Arduino Helpers library.
#include <AH/Hardware/ExtendedInputOutput/SPIShiftRegisterOut.hpp>

using namespace ExtIO; // Bring the ExtIO pin functions into your sketch

// If you wired your LEDs as RGB
const uint8_t ShiftRegisterOutRGB::redBit = 0;
const uint8_t ShiftRegisterOutRGB::greenBit = 1;
const uint8_t ShiftRegisterOutRGB::blueBit = 2;

// If you wired your LEDs as BGR
// const uint8_t ShiftRegisterOutRGB::redBit   = 2;
// const uint8_t ShiftRegisterOutRGB::greenBit = 1;
// const uint8_t ShiftRegisterOutRGB::blueBit  = 0;

// Create a new shift register output connected to pins 11, 13 and 10,
// shift the data out with the most significant bit first.
// There are 24 (= 3×8) outputs in total.
SPIShiftRegisterOut<24> sreg {SPI, SS, MSBFIRST};

void setup() {
  // Initialize the shift registers
  sreg.begin();
}

constexpr unsigned long delayTime = 50;

void loop() {
  // Red on
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(sreg.red(i), HIGH);
    delay(delayTime);
  }
  // Red off
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(sreg.red(i), LOW);
    delay(delayTime);
  }
  // Green on
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(sreg.green(i), HIGH);
    delay(delayTime);
  }
  // Green off
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(sreg.green(i), LOW);
    delay(delayTime);
  }
  // Blue on
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(sreg.blue(i), HIGH);
    delay(delayTime);
  }
  // Blue off
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(sreg.blue(i), LOW);
    delay(delayTime);
  }
}
