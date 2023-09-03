/**
 * Example of using the standard library complex numbers.
 *
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Due, Teensy 3.x, ESP8266, ESP32
 *
 * **Output**
 * ~~~
 * √(2i) = (1.00,1.00)
 * ~~~
 * 
 * Written by PieterP, 2019-11-14
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Arduino_Helpers.h>

#include <AH/STL/complex>

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  // Create a complex number
  std::complex<double> c{0, 2};

  // Print the square root
  Serial << "√(2i) = " << std::sqrt(c) << endl;
}

void loop() {}
