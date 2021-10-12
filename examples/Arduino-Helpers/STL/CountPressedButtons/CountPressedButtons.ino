/**
 * Example of using the standard library algorithms and the Button class.
 *
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, Due, Teensy 3.x, ESP8266, ESP32
 * 
 * This sketch reads a number of push buttons or switches, and uses the standard
 * library algorithms to count how many of them are pressed.
 * 
 * The internal pull-up resistors are enabled and the switches are debounced.
 *
 * Written by PieterP, 2019-11-24  
 * https://github.com/tttapa/Arduino-Helpers
 */
#include <Arduino_Helpers.h>

#include <AH/Hardware/Button.hpp> // Button
#include <AH/STL/numeric>         // std::accumulate

// An array of debounced buttons connected to the given pins
Button buttons[] {
  2, 3, 4, 5, 6, 7, 8, 9,
};

void setup() {
  for (auto &button : buttons) // for all buttons in the array
    button.begin();            // initialize (enable internal pull-up resistor)
  Serial.begin(115200);
}

void loop() {
  // function that adds one to the counter if the given button is pressed
  auto addOneIfPressed = [](unsigned count, const Button &button) {
    return button.getState() == Button::Pressed ? count + 1 : count;
  };

  // read all buttons and debounce them
  for (auto &button : buttons)
    button.update();

  // apply that function to all buttons and return the count
  unsigned pressed =
    std::accumulate(std::begin(buttons), // from the first button
                    std::end(buttons),   // to the last
                    0u,                  // initial value of the counter
                    addOneIfPressed);    // counter function to apply

  Serial.println(pressed);
}
