/**
 * This example reads multiple encoders using pin change interrupts, on an 
 * Arduino Uno or Nano.
 * 
 * @boards AVR
 * 
 * The ATmega328P microcontroller only has two interrupt pins (2 and 3), so if
 * you want to use more than two interrupt-driven encoders, you'll either have 
 * to use a timer interrupt to continuously poll the encoders, or use the chip's
 * pin change interrupts. This example demonstrates the latter.
 *
 * @see @ref Timer-Interrupt-Encoders.ino
 *
 * Familiarity with [direct port manipulation](https://www.arduino.cc/en/Reference/PortManipulation)
 * is assumed.
 *
 * Connections
 * -----------
 *
 * Connect three encoders to the pins of port C as follows:
 * 
 *   - A0: pin A of encoder #0
 *   - A1: pin B of encoder #0
 *   - A2: pin A of encoder #1
 *   - A3: pin B of encoder #1
 *   - A4: pin A of encoder #2
 *   - A5: pin B of encoder #2
 *
 * Connect the common pins to ground, the internal pull-up resistors will be 
 * enabled.
 * 
 * Behavior
 * --------
 *
 * When the position of one of the encoders changes, it is printed to the Serial
 * monitor.
 *
 * Written by PieterP, 2021-08-11  
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Arduino_Helpers.h>
#include <AH/Hardware/RegisterEncoders.hpp>

// The number of encoders to read:
constexpr uint8_t num_enc = 3;
// Mask the bottom 6 bits of the GPIO registers (2 pins × 3 encoders).
// This determines which pins are used:
const uint8_t pin_mask = 0x3F;
// The actual encoder object that keeps track of the encoder state:
RegisterEncoders<uint8_t, num_enc, int32_t, true> encoders;
// AVR uses 8-bit GPIO registers, so the register type is uint8_t.
// Then we specify the number of encoders, the position type,
// and whether the encoders should be interrupt-safe: since we'll
// be calling `encoders.update()` in an interrupt handler, it is 
// important that this is set to true.

// Pin change interrupt handler that reads the pins and updates the state:
ISR (PCINT1_vect) { encoders.update(PINC & pin_mask); } // read port C

void setup() {
  PCMSK1 |= pin_mask; // enable pin change interrupt for given pins in port C
  DDRC &= ~pin_mask;  // input mode for port C
  PORTC |= pin_mask;  // enable pull-up resistors for port C
  PCICR |= 1 << 1;    // enable pin change interrupt for port C
  Serial.begin(115200);
}

void loop() {
  // The previous position of each encoder, to detect changes:
  static int32_t prevPos[num_enc] {};

  // Read the encoder positions and print if they changed:
  for (uint8_t i = 0; i < num_enc; ++i) {
    int32_t newPos = encoders[i].read();
    if (newPos != prevPos[i]) {
      Serial << '[' << i << "] " << newPos << endl;
      prevPos[i] = newPos;
    }
  }
}
