/**
 * This example demonstrates the use of MCP23017 IO expanders as if they were 
 * just normal IO pins.
 * 
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, Due, Teensy 3.x, ESP8266, ESP32
 * 
 * Connections
 * -----------
 * 
 *   - SDA: MCP23017 SDA
 *   - SCK: MCP23017 SCK
 *   - 2:   MCP23017 INT A or INT B
 * 
 * Connect an LED (and series resistor) between pin GPA0 of the MCP and ground.
 * Connect a push button between pin GPB0 of the MCP and ground. The internal
 * pull-up resistor will be used.
 * 
 * Tie the three address lines of the MCP to ground.
 * 
 * Behavior
 * --------
 * 
 * When the push button is pressed, the LED turns on.
 * 
 * Written by PieterP, 2020-11-20  
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Arduino_Helpers.h>
#include <AH/Hardware/ExtendedInputOutput/MCP23017.hpp>
#include <Wire.h>

using namespace ExtIO; // Use the extended input/output functions

using WireType = decltype(Wire); // The type of the I²C driver to use
MCP23017<WireType> mcp {
  Wire, // The I²C driver to use
  0x0,  // The I²C address offset (depends on the state of the address pins)
  2,    // Optional: interrupt pin to detect input changes 
};

pin_t ledPin = mcp.pinA(0);    // GPIOA0
pin_t buttonPin = mcp.pinB(0); // GPIOB0

void setup() {
  Wire.begin();          // Initialize the I²C interface
  Wire.setClock(800000); // Faster I²C clock
  mcp.begin();           // Initialize the MCP23017

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  auto buttonState = digitalRead(buttonPin);
  digitalWrite(ledPin, buttonState ? LOW : HIGH);
}

// Note: 
// The “interrupt” pin is not used to trigger an actual interrupt on the 
// microcontroller. It is just used to check whether any of the inputs of
// the MCP23017 changed without having to do an I²C transaction. Using this
// pin can speed up your code, but it's not necessary, you can leave it out
// in the MCP23017 construction at the top of the sketch.
