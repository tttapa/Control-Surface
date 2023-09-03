/**
 * This examples shows how to use two push buttons to set the frequency at which
 * an LED blinks.
 * 
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Due, Teensy 3.x, ESP8266, ESP32
 * 
 * Connections
 * -----------
 * 
 * - 2: Momentary push button (other pin to ground)
 * - 3: Momentary push button (other pin to ground)
 * 
 * The internal pull-up resistors will be enabled.
 * 
 * Behavior
 * --------
 * 
 * - If you press the first push button, the LED blinks faster.
 * - If you press the second push button, the LED blinks slower.
 * - You can press and hold one of the push buttons to change the frequency by
 *   multiple steps.
 * - If you press both buttons at the same time, the frequency is reset to the
 *   initial default frequency.
 * 
 * Written by PieterP, 2019-12-10  
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Arduino_Helpers.h>

#include <AH/Hardware/IncrementDecrementButtons.hpp>
#include <AH/Timing/MillisMicrosTimer.hpp>

const unsigned long maxInterval = 2000;    // ms
const unsigned long minInterval = 100;     // ms
const unsigned long defaultInterval = 500; // ms
const int intervalDelta = 100;             // ms

IncrementDecrementButtons buttons {2, 3};
Timer<millis> timer = defaultInterval;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  buttons.begin();
  Serial.begin(115200);
}

void loop() {
  // toggle the LED when the given number of milliseconds have passed
  if (timer)
    digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) ? LOW : HIGH);

  // read the buttons, and change interval accordingly
  switch (buttons.update()) {
    case IncrementDecrementButtons::IncrementShort:
    case IncrementDecrementButtons::IncrementLong:
    case IncrementDecrementButtons::IncrementHold:
      timer.setInterval(
        AH::max(timer.getInterval() - intervalDelta, minInterval));
      break;
    case IncrementDecrementButtons::DecrementShort:
    case IncrementDecrementButtons::DecrementLong:
    case IncrementDecrementButtons::DecrementHold:
      timer.setInterval(
        AH::min(timer.getInterval() + intervalDelta, maxInterval));
      break;
    case IncrementDecrementButtons::Reset:
      timer.setInterval(defaultInterval);
      break;
    default: break;
  }

  // print the new interval if a button was pushed
  if (buttons.getState() != IncrementDecrementButtons::Nothing)
    Serial.println(timer.getInterval());
}