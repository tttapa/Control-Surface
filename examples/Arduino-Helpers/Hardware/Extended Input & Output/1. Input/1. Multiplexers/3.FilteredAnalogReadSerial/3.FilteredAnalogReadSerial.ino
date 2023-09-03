/**
 * This is an example of the AnalogMultiplex and the FilteredAnalog classes. 
 * It prints the filtered values of all 16 inputs of a multiplexers to the 
 * serial monitor.
 * 
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Due, Teensy 3.x, ESP8266, ESP32
 * 
 * Connections
 * -----------
 * 
 *  - A0: CD74HC4067 signal pin ("common input/output")
 *  - 3:  CD74HC4067 address pin S0
 *  - 4:  CD74HC4067 address pin S1
 *  - 5:  CD74HC4067 address pin S2
 *  - 6:  CD74HC4067 address pin S3
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
 * Behavior
 * --------
 * 
 * Connect some potentiometers or other analog sensors to the 16 inputs
 * of the multiplexer, and open the serial monitor (CTRL+SHIFT+M) or the
 * serial plotter (CTRL+SHIFT+L). You should see all 16 signals printed
 * or plotted.
 * 
 * Written by Pieter P, 09-04-2019  
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Arduino_Helpers.h> // Include the Arduino Helpers library

#include <AH/Hardware/ExtendedInputOutput/AnalogMultiplex.hpp>
#include <AH/Hardware/FilteredAnalog.hpp>
#include <AH/Containers/ArrayHelpers.hpp> // copyAs<>

// Instantiate a multiplexer
CD74HC4067 mux {
  A0,           // analog pin
  {3, 4, 5, 6}, // Address pins S0, S1, S2, S3
  // 7, // Optionally, specify the enable pin
};

// Alternatively, if you have a 3-bit mux:
// CD74HC4051 mux {
//   A0,
//   {3, 4, 5},
//   // 7, // Optional
// };

// Convert the list of pins of the multiplexer to an array
// of FilteredAnalog objects.
auto filteredAnalogs = copyAs<FilteredAnalog<>>(mux.pins());

void setup() {
  Serial.begin(115200);
  mux.begin(); // Initialize multiplexer
}

void loop() {
  // Loop over all FilteredAnalog objects in the array
  for (auto &analog : filteredAnalogs) {
    analog.update(); // actually read the analog value and filter it
    Serial.print(analog.getValue());
    Serial.print('\t');
  }
  Serial.println();
}
