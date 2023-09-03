/**
 * This example demonstrates the use of MAX7219 LED outputs as if they were just
 * normal IO pins, using `digitalWrite`.
 * 
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Due, Teensy 3.x, ESP8266, ESP32
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
 * This sketch will blink the first and the last LEDs once a second, in an 
 * alternating pattern.
 * 
 * Written by PieterP, 2020-03-24  
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Arduino_Helpers.h> // Include the Arduino Helpers library.
#include <AH/Hardware/ExtendedInputOutput/MAX7219.hpp>

using namespace ExtIO; // Bring the ExtIO pin functions into your sketch

// Instantiate a MAX7219 with the SPI slave select pin as latch pin
// There's just 1 MAX7219 in the chain, if you have more of them daisy-chained
// together, you can increase the template argument (between angled brackets)
MAX7219<1> max7219 {SPI, SS};

const pin_t ledPin1 = max7219.pin(0);  // first LED of the MAX7219
const pin_t ledPin2 = max7219.pin(63); // last LED of the MAX7219

void setup() {
  max7219.begin();          // Initialize the shift registers
  pinMode(ledPin1, OUTPUT); // You don't even need this line, since
  pinMode(ledPin2, OUTPUT); // since the MAX7219's pins are always outputs
}

void loop() {
  // Toggle the state of the LEDs every 1/2 second
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, LOW);
  delay(500);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, HIGH);
  delay(500);
}
