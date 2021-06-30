/**
 * This example demonstrates the use of shift registers as if they were just
 * normal IO pins. This version uses bit-banging to drive the  shift register. 
 * You should probably be using the SPI version instead.
 * 
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, Due, Teensy 3.x, ESP8266, ESP32
 * 
 * Connections
 * -----------
 * 
 *   - 10: 74HC595 ST_CP
 *   - 11: 74HC595 DS
 *   - 13: 74HC595 SH_CP
 * 
 * Connect an LED (and series resistor) between the first output of the
 * shift register and ground.
 * 
 * Remember to connect the enable pin the shift register to ground and the 
 * master reset pin to Vcc in order to enable it.  
 * 
 * Behavior
 * --------
 * 
 * This sketch will blink the LED once a second.
 * 
 * Written by PieterP, 2018-09-01
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Arduino_Helpers.h> // Include the Arduino Helpers library.
#include <AH/Hardware/ExtendedInputOutput/ShiftRegisterOut.hpp>

using namespace ExtIO; // Bring the ExtIO pin functions into your sketch

const pin_t latchPin = 10; // Pin connected to ST_CP of 74HC595
const pin_t dataPin = 11;  // Pin connected to DS of 74HC595
const pin_t clockPin = 13; // Pin connected to SH_CP of 74HC595

// Instantiate a shift register on the correct pins, most significant bit first,
// and a total of 8 outputs.
ShiftRegisterOut<8> sreg {dataPin, clockPin, latchPin, MSBFIRST};

const pin_t ledPin = sreg.pin(0); // first pin of the shift register

void setup() {
  sreg.begin();            // Initialize the shift registers
  pinMode(ledPin, OUTPUT); // You don't even need this line, since
                           // shift registers are always outputs
}

void loop() {
  // Toggle the state of the LED every 1/2 second
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
}
