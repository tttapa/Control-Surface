/**
 * This examples shows how to use the debounced Button class to toggle an LED.
 * 
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Due, Teensy 3.x, ESP8266, ESP32
 * 
 * Connections
 * -----------
 * 
 * - 2: Momentary push button (other pin to ground)
 * 
 * The internal pull-up resistor will be enabled.
 * 
 * Behavior
 * --------
 * 
 * - If you press the push button once, the built-in LED is turned on.
 * - If you press the button again, the LED is turned off again.
 * 
 * Written by PieterP, 2019-11-22  
 * https://github.com/tttapa/Arduino-Helpers
 */

// Include the library
#include <Arduino_Helpers.h>

#include <AH/Hardware/Button.hpp>

// Create a Button object that reads a push button connected to pin 2:
Button pushbutton {2};

// The pin with the LED connected:
const pin_t ledPin = LED_BUILTIN;

void setup() {
  pinMode(ledPin, OUTPUT);
  pushbutton.begin();
  // You can invert the input, for use with normally closed (NC) switches:
  // pushbutton.invert();
}

void loop() {
  static bool ledState = LOW;
  // Read the digital input, debounce the signal, and check the state of
  // the button:
  if (pushbutton.update() == Button::Falling) {
    ledState = !ledState; // Invert the state of the LED
    // Update the LED with the new state
    digitalWrite(ledPin, ledState ? HIGH : LOW);
  }
}
