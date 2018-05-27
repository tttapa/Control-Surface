/**
 * @example Ex.03.Rotary-Encoder.ino
 * 
 * This is an example of the RotaryEncoder class of the Control_Surface library.
 * 
 * Connect the A and B pins of the encoder to 2 interrupt pins (2 and 3).  
 * It's recommended to use 100nF capacitors between the A and B pins and ground (hardware debounce).  
 * Connect the common (C) pin to the ground. Pull-up resistors are not necessary, since the internal ones will be used.  
 * Map the control change message 0x14 to the right control in your DJ or DAW software, 
 * and select 'relative' instead of 'absolute'.
 *
 * If you are using a jog wheel, use JOG, if you are using a normal encoder 
 * (for normal controls like EQ or volume etc.) use NORMAL_ENCODER.  
 * If you have strange results in your software, try a different relative mode: 
 * TWOS_COMPLEMENT, BINARY_OFFSET or SIGN_MAGNITUDE.  
 * If the control works, but it goes in the wrong direction, swap the pins A and B of the encoder 
 * (either physically, or by swapping the pin numbers in the RotaryEncoder constructor).
 *
 * Written by Pieter P, 22-08-2017  
 * https://github.com/tttapa/Control-Surface
*/

#include <Control_Surface.h>

const uint8_t Channel = 1;       // MIDI channel 1
const uint8_t Controller = 0x14; // MIDI Control Change controller number
const int speedMultiply = 1;     // No change in speed of the encoder (number of steps is multiplied by 1)

// Create a new instance of the class 'RotaryEncoder', called 'enc', on pin 2 and 3, controller number 0x14, on channel1,
// no change in speed (speed is multiplied by 1), it's used as a jog wheel, and the sign mode is set to two's complement.
RotaryEncoder encoder(2, 3, Controller, Channel, speedMultiply, JOG, TWOS_COMPLEMENT);

void setup() {}

void loop() {
  // Refresh the encoder (check whether the position has changed since last time, if so, send the difference over MIDI)
  Control_Surface.refresh();
}
