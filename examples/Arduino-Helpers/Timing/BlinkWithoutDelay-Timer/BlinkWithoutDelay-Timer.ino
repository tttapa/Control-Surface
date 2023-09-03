/**
 * This examples shows how use the Timer class to blink an LED every
 * second, in a non-blocking fashion (without using delay).
 * 
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Due, Teensy 3.x, ESP8266, ESP32
 * 
 * Behavior
 * --------
 * 
 * Blinks the built-in LED every second.
 * 
 * Written by PieterP, 2019-12-06  
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Arduino_Helpers.h>
#include <AH/Timing/MillisMicrosTimer.hpp>

Timer<millis> timer = 500; // milliseconds

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (timer) {
    digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) ? LOW : HIGH);
  }
}
