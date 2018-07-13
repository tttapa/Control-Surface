/**
 * @example Ex.1.Shift-Register-LED-Effects.ino
 * 
 * This is an example of the ShiftRegisterOut class of the Control-Surface library.
 *
 * Connect three daisy-chained shift registersr to pins 11 (ST_CP), 12 (DS) and 10 (SH_CP).  
 * Connect 8 RGB LEDs (+ current limiting resistors) to the outputs of the shift registers.
 * 
 * ```
 * 10 >──────────────┬─────────────────────────────┬─────────────────────────────┐            
 *        ┎━━━━━━━━━━┷━━━━━━━━━━━┓      ┎━━━━━━━━━━┷━━━━━━━━━━━┓      ┎━━━━━━━━━━┷━━━━━━━━━━━┓
 *        ┃        SH_CP         ┃      ┃        SH_CP         ┃      ┃        SH_CP         ┃
 * 12 >───┨ Data in     Data out ┠──────┨ Data in     Data out ┠──────┨ Data in     Data out ┃
 *        ┃        ST_CP         ┃      ┃        ST_CP         ┃      ┃        ST_CP         ┃
 *        ┗━━━━━━━━━━┯━━━━━━━━━━━┛      ┗━━━━━━━━━━┯━━━━━━━━━━━┛      ┗━━━━━━━━━━┯━━━━━━━━━━━┛
 * 11 >──────────────┴─────────────────────────────┴─────────────────────────────┘            
 * ```
 * 
 * The order of the colors doesn't matter. You can change them in software using the
 * `ShiftRegisterOut::redBit`, `ShiftRegisterOut::greenBit` and `ShiftRegisterOut::blueBit`
 * constants.
 * 
 * If you wired the LEDs as RGB (red first, then green and then blue), the red bit is 0,
 * the green bit is 1 and the blue bit is 2.  
 * If you wired the LEDs as BGR (blue first, then green and then red), the red bit is 2,
 * the green bit is 1 and the blue bit is 0.  
 * Other combinations are possible as well.
 * 
 * This example will turn on all red LEDs one by one, then turn them off one by one, 
 * next, it will turn on and off all green LEDs in the same manner, and finally the
 * same for the blue LEDs. This is repeated indefinitely.
 * 
 * Written by Pieter P, 13-07-2018  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the library

using namespace ExtIO;

const pin_t dataPin  = 11;  // Pin connected to DS of 74HC595
const pin_t clockPin = 13;  // Pin connected to SH_CP of 74HC595
const pin_t latchPin = 10;  // Pin connected to ST_CP of 74HC595

/* If you wired your LEDs as RGB */
// const uint8_t ShiftRegisterOut::redBit = 0;
// const uint8_t ShiftRegisterOut::greenBit = 1;
// const uint8_t ShiftRegisterOut::blueBit = 2;

/* If you wired your LEDs as BGR */
const uint8_t ShiftRegisterOut::redBit   = 2;
const uint8_t ShiftRegisterOut::greenBit = 1;
const uint8_t ShiftRegisterOut::blueBit  = 0;

// Create a new shift register output connected to pins 11, 13 and 10,
// shift the data out with the most significant bit first.
// There are 24 (= 3·8) outputs in total.
ShiftRegisterOut ShiftReg(dataPin, clockPin, latchPin, MSBFIRST, 24);

void setup() {
  // Initialize the shift register
  ShiftReg.begin();
}

const unsigned long delayTime = 50;

void loop() {
  // Red on
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(ShiftReg.red(i), HIGH);
    delay(delayTime);
  }
  // Red off
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(ShiftReg.red(i), LOW);
    delay(delayTime);
  }
  // Green on
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(ShiftReg.green(i), HIGH);
    delay(delayTime);
  }
  // Green off
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(ShiftReg.green(i), LOW);
    delay(delayTime);
  }
  // Blue on
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(ShiftReg.blue(i), HIGH);
    delay(delayTime);
  }
  // Blue off
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(ShiftReg.blue(i), LOW);
    delay(delayTime);
  }
}

