/**
 * Example of using the standard library numeric functions.
 *
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Due, Teensy 3.x, ESP8266, ESP32
 *
 * This sketch creates an incremental array using the iota function, then
 * computes the inner product of the vector with itself, and uses that to
 * calculate the norm of the vector.
 *
 * **Output**
 * ~~~
 * v      = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
 * 〈v, v〉 = 385
 * ‖v‖    = 19.6214160
 * ~~~
 * 
 * Written by PieterP, 2019-11-18  
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Arduino_Helpers.h>

#include <AH/Containers/Array.hpp>        // Array
#include <AH/Containers/ArrayHelpers.hpp> // For printing the array
#include <AH/STL/cmath>                   // std::sqrt
#include <AH/STL/numeric>                 // std::iota, std::inner_product

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  // Create an array with the elements [1, 2 ... 9, 10]
  Array<int, 10> vector;
  std::iota(std::begin(vector), std::end(vector), 1);
  Serial << "v      = [" << vector << ']' << endl;

  // Compute the inner product of vector with itself, with an initial sum of 0
  int sum_sq = std::inner_product(std::begin(vector), std::end(vector),
                                  std::begin(vector), 0);
  Serial << "〈v, v〉 = " << sum_sq << endl;

  // The 2-norm of a vector is the squareroot of the inner product with itself
  double norm = std::sqrt(sum_sq);
  Serial << "‖v‖    = " << setprecision(7) << norm << endl;
}

void loop() {}
