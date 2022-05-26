/**
 * This example demonstrates the use of MCP23017 I²C port expanders with rotary
 * encoders, using hardware interrupts so you don't miss any pulses.
 * 
 * @boards  Teensy 3.x
 * 
 * This only works on Teensy boards, and maybe on other ARM boards.  
 * I've tested it using a Teensy 3.2 and a Teensy 4.0.
 * 
 * AVR boards (Arduino Uno, Mega, etc.) don't support I²C inside of interrupt
 * service routines, so there's no way to read the state of the MCP23017
 * inside of an ISR.  
 * The same goes for ESP32 and ESP8266, although there may be ways around
 * this by using alternative I²C drivers or by using FreeRTOS features.
 * 
 * Connections
 * -----------
 * 
 *   - SDA:  MCP23017 SDA
 *   - SCL:  MCP23017 SCL
 *   - 12:   MCP23017 INTA or INTB
 *   
 * Connect up to 8 encoders to the MCP23017's GPIO pins: 
 * the first encoder connects to GPIO A0 and A1, the second encoder connects to 
 * GPIO A2 and A3, ..., the eighth encoder connects to GPIO B6 and B7.  
 * Connect the "common" pins of the encoders to ground.  
 * The built-in pull-up resistors of the MCP23017 will be enabled.
 * 
 * Make sure that the reset and address pins are all configured correctly 
 * (reset to Vcc; A0, A1 and A2 to ground).
 * 
 * If you need more than one MCP23017 with encoders, you can connect their 
 * interrupt pins together. This does result in higher latency, because on 
 * average, half of the total number of MCP23017s have to be polled, which is 
 * relatively slow, and might lead to missed pulses when using a large number
 * of encoders.
 * 
 * Behavior
 * --------
 * 
 * When the position of one of the encoders changes, it is printed to the Serial
 * monitor.
 * 
 * The interrupt pin of the MCP23017 triggers a hardware interrupt on the
 * Arduino. This means that you don't have to manually poll the encoders all
 * the time.
 * 
 * Written by PieterP, 2020-04-06  
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Wire.h>

#include <Arduino_Helpers.h>

#include <AH/Hardware/MCP23017Encoders.hpp>
#include <AH/PrintStream/PrintStream.hpp>

using WireType = decltype(Wire);     // The type of I²C driver to use
using EncoderPositionType = int32_t; // The type for saving encoder positions
constexpr bool IntSafe = true;       // Make it safe to call `update` in an ISR
using MCPEncoderType = MCP23017Encoders<WireType, EncoderPositionType, IntSafe>;

const uint8_t interrupt_pin = 12;

MCPEncoderType enc {Wire, 0x0, interrupt_pin};
//                  │     │    └─ Interrupt pin
//                  │     └────── Address offset
//                  └──────────── I²C interface

void isr() {
  enc.update(); // Read the state of the encoders and update the positions
}

void setup() {
  Serial.begin(115200);
  Wire.begin(); // Must be called before enc.begin()
  Wire.setClock(800000);
  enc.begin(); // Initialize the MCP23017
  attachInterrupt(digitalPinToInterrupt(interrupt_pin), isr, LOW);
}

void loop() {
  // No `enc.update()` here, everything happens inside of the ISR

  // Save the previous positions
  static EncoderPositionType prevPositions[8] {};
  // Check if an encoder position changed and print it
  for (uint8_t i = 0; i < 8; ++i) {
    auto newPosition = enc.read(i);
    if (newPosition != prevPositions[i]) {
      Serial << '[' << i << "] " << newPosition << endl;
      prevPositions[i] = newPosition;
    }
  }
}
