/**
 * @brief   This is an example of the AnalogMultiplex class.
 * 
 * ### Connections
 *  - 9:  CD74HC4067 address pin S0
 *  - 10: CD74HC4067 address pin S1
 *  - 11: CD74HC4067 address pin S2
 *  - 12: CD74HC4067 address pin S3
 *  - A0: CD74HC4067 signal pin ("common input/output")
 *  
 * Optionally you can connect the enable pin as well, this is useful
 * if you want to use multiple multiplexers with the same address 
 * lines and the same analog input. Otherwise, just connect the enable
 * pin to ground.
 * 
 * If you are using a 3-bit multiplexer, like the CD74HC4051, you can
 * uncomment the code specific to this multiplexer, and only use three
 * address pins.
 *  
 * ### Behavior
 * Connect some potentiometers or other analog sensors to the 16 inputs
 * of the multiplexer, and open the serial monitor (CTRL+SHIFT+M) or the
 * serial plotter (CTRL+SHIFT+L). You should see all 16 signals printed
 * or plotted.
 * 
 * Written by Pieter P, 31-01-2019  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

CD74HC4067 mux = { 
  A0,               // analog pin
  {9, 10, 11, 12},  // Address pins S0, S1, S2, S3
  // 13, // Optionally, specify the enable pin
};

// Alternatively, if you have a 3-bit mux:
// CD74HC4051 mux = { 
//   A0, 
//   {9, 10, 11}, 
//   // 13, // Optional
// };

void setup() {
    Serial.begin(115200);
    mux.begin(); // Initialize multiplexer
}

void loop() {
  for (pin_t pin = 0; pin < mux.getLength(); ++pin) {
    Serial.print(mux.analogRead(pin));
    Serial.print('\t');
  }
  Serial.println();  
}
