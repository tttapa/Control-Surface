/**
 * This examples shows how to filter an analog input, so you can get the 
 * position of a knob or fader without noise.
 * 
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, Due, Teensy 3.x, ESP8266, ESP32
 * 
 * Connections
 * -----------
 * 
 * - A0: wiper of a potentiometer
 * 
 * Connect the left terminal of the potentiometer to ground, and the right one
 * to V<sub>CC</sub>.
 * 
 * Behavior
 * --------
 * 
 * - Upload the sketch to the Arduino, and open the Serial Monitor 
 *   (`CTRL+Shift+M`)
 * - When you turn the potentiometer, you should see the position of the 
 *   potentiometer being printed as a number between 0 and 1023.
 * - The analog input is filtered, so there shouldn't be any noise on the 
 *   position. If there is, check your wiring, and make sure that the resistance
 *   of the potentiometer isn't too high (10 kΩ is ideal). 
 * 
 * A simpler version of this example can be found in 
 * @ref 1.FilteredAnalog.ino.
 * 
 * Written by PieterP, 2019-10-10  
 * https://github.com/tttapa/Arduino-Helpers
 */

// Include the library
#include <Arduino_Helpers.h>

#include <AH/Hardware/FilteredAnalog.hpp>
#include <AH/Timing/MillisMicrosTimer.hpp>

// Create a filtered analog object on pin A0:
FilteredAnalog<10,       // Output precision in bits
               2,        // The amount of filtering
               uint16_t, // The integer type for the filter calculations
               analog_t  // The integer type for the upscaled analog values
              >
  analog = A0;

// If you want more filtering, you can increase the filter shift factor.
// The number of bits required for the intermediate calculations increases if
// you do so, so you have to use a larger type as well.
FilteredAnalog<10,      // Output precision in bits
               6,       // The amount of filtering
               uint32_t // The integer type for the filter calculations
              >
  moreFiltering = A0;

// If you don't care about the specific settings, and just want a
// default that works, you can use the following:
FilteredAnalog<> simpleAnalog = A0;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  // Select the correct ADC resolution
  FilteredAnalog<>::setupADC();
  // If you want, you can add mapping functions to invert the input, for example
  analog.invert();
  // Initialize the filter to whatever the value on the input is right now
  // (otherwise, the filter is initialized to zero and you get transients)
  analog.resetToCurrentValue();
}

void loop() {
  // Read the analog input every millisecond, and print if the value has changed
  static Timer<millis> timer = 1; // ms
  if (timer && analog.update())
    Serial.println(analog.getValue());

  // Explanation:
  //
  // analog.update() reads the analog input, applies the filters,
  // saves the value, and returns true if the value has changed.
  // You can then retreive the new value using analog.getValue().
  //
  // Timer is just a "Blink Without Delay" wrapper, it returns true
  // every time the specified amount of time has passed.
}
