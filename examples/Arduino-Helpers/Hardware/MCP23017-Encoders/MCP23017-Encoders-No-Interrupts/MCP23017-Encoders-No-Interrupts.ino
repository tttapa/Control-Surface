/**
 * This example demonstrates the use of MCP23017 I²C port expanders with rotary
 * encoders.
 * 
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, Due, Teensy 3.x, ESP8266, ESP32
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
 * Cautions
 * --------
 * 
 * The interrupt pin of the MCP23017 doesn't trigger a hardware interrupt on the
 * Arduino. It's used only to determine whether the Arduino should read the GPIO
 * registers from the MCP23017.  
 * If the update function isn't called often enough, you will miss encoder 
 * pulses, and the position will be incorrect.  
 * It's very important to keep your main loop free from any code that blocks for
 * more than a couple of hundreds of microseconds.
 * 
 * Written by PieterP, 2020-04-06  
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Arduino_Helpers.h>

#include <AH/Hardware/MCP23017Encoders.hpp>
#include <AH/PrintStream/PrintStream.hpp>

#include <Wire.h>

using WireType = decltype(Wire);     // The type of I²C driver to use
using EncoderPositionType = int32_t; // The type for saving encoder positions
using MCPEncoderType = MCP23017Encoders<WireType, EncoderPositionType>;

MCPEncoderType enc {Wire, 0x0, 12};
//                  │     │    └─ Interrupt pin
//                  │     └────── Address offset
//                  └──────────── I²C interface

void setup() {
  Serial.begin(115200);
  Wire.begin(); // Must be called before enc.begin()
  Wire.setClock(800000);
  enc.begin(); // Initialize the MCP23017
}

void loop() {
  enc.update(); // Read the state of the encoders and update the positions

  // Save the previous positions
  static EncoderPositionType prevPositions[8] {};
  // Check if an encoder position changed and print it
  for (uint8_t i = 0; i < 8; ++i) {
    if (enc.read(i) != prevPositions[i]) {
      Serial << '[' << i << "] " << enc.read(i) << endl;
      prevPositions[i] = enc.read(i);
    }
  }
}
