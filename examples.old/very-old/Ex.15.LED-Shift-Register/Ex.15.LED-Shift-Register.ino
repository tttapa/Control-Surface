/**
 * @example Ex.15.LED-Shift-Register.ino
 * 
 * This is an example of the ShiftRegisterOut class of the Control_Surface library.
 *
 * Connect three daisy-chained shift registers to pins 11 (ST_CP), 12 (DS) and 10 (SH_CP).  
 * Connect 8 RGB LEDs (+ current limiting resistors) to the outputs of the shift registers
 * (R-G-B-R-G-B-...).  
 * If blue LEDs light up when red ones should, either flip all the LEDs, or change the 
 * bit order in the ShiftRegisterOut constructor to 'MSBFIRST' instead fo 'LSBFIRST'.
 * 
 * ```
 * 10 >──────────────┬─────────────────────────────┬─────────────────────────────┐            
 *        ┏━━━━━━━━━━┷━━━━━━━━━━━┓      ┏━━━━━━━━━━┷━━━━━━━━━━━┓      ┏━━━━━━━━━━┷━━━━━━━━━━━┓
 *        ┃        SH_CP         ┃      ┃        SH_CP         ┃      ┃        SH_CP         ┃
 * 12 >───┨ Data in     Data out ┠──────┨ Data in     Data out ┠──────┨ Data in     Data out ┃
 *        ┃        ST_CP         ┃      ┃        ST_CP         ┃      ┃        ST_CP         ┃
 *        ┗━━━━━━━━━━┯━━━━━━━━━━━┛      ┗━━━━━━━━━━┯━━━━━━━━━━━┛      ┗━━━━━━━━━━┯━━━━━━━━━━━┛
 * 11 >──────────────┴─────────────────────────────┴─────────────────────────────┘            
 * ```
 * The 8 LEDs represent 8 channels. A red LED indicates that the channel is muted, 
 * a green LED indicates that solo is enabled, and a blue LED indicates that the 
 * channel is armed for recording.
 *
 * Configure as a MCU-compatible Control Surface (Mackie Control Universal) in your DAW.
 *
 * Written by Pieter P, 22-03-2018  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the library

const pin_t dataPin  = 12; //Pin connected to DS of 74HC595
const pin_t clockPin = 10; //Pin connected to SH_CP of 74HC595
const pin_t latchPin = 11; //Pin connected to ST_CP of 74HC595

// Create a new shift register output connected to pins 11, 13 and 10, shift the data out with the least significant bit first, 
// There are 24 (= 3·8) outputs in total.
ShiftRegisterOut ShiftReg(dataPin, clockPin, latchPin, LSBFIRST, 24);

const uint8_t channel = 1;

MIDI_LED leds[] = {
  {ShiftReg.red(0), MCU::MUTE_1, channel},
  {ShiftReg.red(1), MCU::MUTE_2, channel},
  {ShiftReg.red(2), MCU::MUTE_3, channel},
  {ShiftReg.red(3), MCU::MUTE_4, channel},
  {ShiftReg.red(4), MCU::MUTE_5, channel},
  {ShiftReg.red(5), MCU::MUTE_6, channel},
  {ShiftReg.red(6), MCU::MUTE_7, channel},
  {ShiftReg.red(7), MCU::MUTE_8, channel},
  {ShiftReg.green(0), MCU::SOLO_1, channel},
  {ShiftReg.green(1), MCU::SOLO_2, channel},
  {ShiftReg.green(2), MCU::SOLO_3, channel},
  {ShiftReg.green(3), MCU::SOLO_4, channel},
  {ShiftReg.green(4), MCU::SOLO_5, channel},
  {ShiftReg.green(5), MCU::SOLO_6, channel},
  {ShiftReg.green(6), MCU::SOLO_7, channel},
  {ShiftReg.green(7), MCU::SOLO_8, channel},
  {ShiftReg.blue(0), MCU::REC_RDY_1, channel},
  {ShiftReg.blue(1), MCU::REC_RDY_2, channel},
  {ShiftReg.blue(2), MCU::REC_RDY_3, channel},
  {ShiftReg.blue(3), MCU::REC_RDY_4, channel},
  {ShiftReg.blue(4), MCU::REC_RDY_5, channel},
  {ShiftReg.blue(5), MCU::REC_RDY_6, channel},
  {ShiftReg.blue(6), MCU::REC_RDY_7, channel},
  {ShiftReg.blue(7), MCU::REC_RDY_8, channel},
};

void setup() {} // Nothing to set up

void loop() {
  // Refresh the Control Surface (read MIDI input and update LEDs).
  Control_Surface.refresh();
}
