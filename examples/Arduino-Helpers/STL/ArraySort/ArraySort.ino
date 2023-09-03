/**
 * Example of using the standard library algorithms.
 *
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Due, Teensy 3.x, ESP8266, ESP32
 *
 * This sketch creates an array with an unordered sequence of 10 numbers.
 * It prints the array, and then creates a reverse view on the 6 elements in the 
 * middle of the array and sorts it.
 * The resulting array has the same elements in the 4 outermost places, but the
 * 6 elements in the middle have been sorted in reverse order.
 *
 * **Output**
 * ~~~
 * Before sorting: 3, 5, 9, 2, 1, 8, 6, 4, 7, 0
 *   Sorting            ├─────────────────┤      in ascending order
 * After sorting:  3, 5, 1, 2, 4, 6, 8, 9, 7, 0
 *   Sorting            ├─────────────────┤      in descending order
 * After sorting:  3, 5, 9, 8, 6, 4, 2, 1, 7, 0
 * ~~~
 * 
 * Written by PieterP, 2019-11-12
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Arduino_Helpers.h>

#include <AH/Containers/Array.hpp>        // Array
#include <AH/Containers/ArrayHelpers.hpp> // For printing the array
#include <AH/STL/algorithm>               // std::sort

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Array<int, 10> array {3, 5, 9, 2, 1, 8, 6, 4, 7, 0};

  Serial << "Before sorting: " << array << endl;

  Serial << "  Sorting            ├─────────────────┤      in ascending order"
         << endl;

  // select elements 2 through 7 (inclusive)
  auto forward_view = array.slice<2, 7>();
  // sort the view in ascending order
  std::sort(std::begin(forward_view), std::end(forward_view));

  Serial << "After sorting:  " << array << endl;
  Serial << "  Sorting            ├─────────────────┤      in descending order"
         << endl;

  // select elements 2 through 7 (inclusive) in reverse order
  auto reverse_view = array.slice<7, 2>();
  // sort the reverse view in ascending order (this means that the original
  // array is sorted in descending order)
  std::sort(std::begin(reverse_view), std::end(reverse_view));

  Serial << "After sorting:  " << array << endl;
}

void loop() {}
