/**
 * This is an example of the AnalogMultiplex class. It prints the states of all
 * 16 switches connected to a multiplexers to the serial monitor.
 * 
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Due, Teensy 3.x, ESP8266, ESP32
 * 
 * Connections
 * -----------
 * 
 *  - 2: CD74HC4067 signal pin ("common input/output")
 *  - 3: CD74HC4067 address pin S0
 *  - 4: CD74HC4067 address pin S1
 *  - 5: CD74HC4067 address pin S2
 *  - 6: CD74HC4067 address pin S3
 *  
 * Optionally you can connect the enable pin as well, this is useful
 * if you want to use multiple multiplexers with the same address 
 * lines and the same analog input. Otherwise, just connect the enable
 * pin to ground.
 * 
 * If you are using a 3-bit multiplexer, like the CD74HC4051, you can
 * uncomment the code specific to this multiplexer, and use only three
 * address pins.
 * 
 * Connect a switch or push button between each input pin of the multiplexer and
 * ground. A pull-up resistor is not necessary, because we'll use the internal 
 * one.
 *  
 * Behavior
 * --------
 * 
 * Open the serial monitor (CTRL+SHIFT+M) or the serial plotter (CTRL+SHIFT+L),
 * and press some buttons, you should see all 16 signals printed or plotted.
 * 
 * Written by Pieter P, 2019-08-08   
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Arduino_Helpers.h> // Include the Arduino Helpers library
#include <AH/Hardware/ExtendedInputOutput/AnalogMultiplex.hpp>

// Instantiate a multiplexer
CD74HC4067 mux {
  2,               // input pin
  {3, 4, 5, 6}, // Address pins S0, S1, S2, S3
  // 7, // Optionally, specify the enable pin
};

// Alternatively, if you have a 3-bit mux:
// CD74HC4051 mux {
//   2,
//   {3, 4, 5},
//   // 7, // Optional
// };

void setup() {
  Serial.begin(115200);
  mux.begin();                  // Initialize multiplexer
  mux.pinMode(0, INPUT_PULLUP); // Set the pin mode (setting it for one pin of
                                // the multiplexers sets it for all of them)
}

void loop() {
  for (pin_t pin = 0; pin < mux.getLength(); ++pin) {
    Serial.print(mux.digitalRead(pin));
    Serial.print('\t');
  }
  Serial.println();
}

// A more fancy approach could be to use a range-based for loop
void loop2() {
  for (pin_t pin : mux.pins()) {
    Serial.print(digitalRead(pin));
    Serial.print('\t');
  }
  Serial.println();
}
// Okay, it's a bit slower, because it has to look up what ExtIO device the pin
// belongs to, but hey, it's nice to have it anyway
