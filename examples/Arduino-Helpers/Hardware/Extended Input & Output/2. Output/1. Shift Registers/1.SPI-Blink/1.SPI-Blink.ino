/**
 * This example demonstrates the use of shift registers as if they were just
 * normal IO pins. The SPI interface is used because it's easy and fast.
 * 
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Due, Teensy 3.x, ESP8266, ESP32
 * 
 * Connections
 * -----------
 * 
 *   - SS:   74HC595 ST_CP
 *   - MOSI: 74HC595 DS
 *   - SCK:  74HC595 SH_CP
 * 
 * Connect an LED (and series resistor) between the first output of the
 * shift register and ground.
 * 
 * Remember to connect the enable pin of the shift register to ground and the 
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
#include <AH/Hardware/ExtendedInputOutput/SPIShiftRegisterOut.hpp>

using namespace ExtIO; // Bring the ExtIO pin functions into your sketch

// Instantiate a shift register with the SPI slave select pin as latch pin, most
// significant bit first, and a total of 8 outputs.
SPIShiftRegisterOut<8> sreg {SPI, SS, MSBFIRST};

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
