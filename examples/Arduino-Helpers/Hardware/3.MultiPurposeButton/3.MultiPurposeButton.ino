/**
 * This examples shows how to use the MultiPurposeButton class to detect 
 * different kinds of push button events.
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
 * - Short presses, long presses, multiple presses, etc. are printed to the 
 *   Serial monitor.
 * 
 * Written by PieterP, 2022-05-07  
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Arduino_Helpers.h> // https://github.com/tttapa/Arduino-Helpers

#include <AH/Hardware/MultiPurposeButton.hpp>

// Create a Button object that reads a push button connected to pin 2:
MultiPurposeButton btn{2};

void setup() {
  Serial.begin(115200);
  btn.setLongPressDelay(1000);
  btn.setMultiPressDelay(400);
  btn.begin();
}

void loop() {
  auto longPressTime = btn.getLongPressedTime();
  switch (btn.update()) {
    case btn.None: break;
    case btn.PressStart: Serial << "Pressed" << endl; break;
    case btn.ShortPressRelease:
      Serial << "Released after short press" << endl;
      break;
    case btn.LongPress: Serial << "Long press" << endl; break;
    case btn.LongPressRelease:
      Serial << "Released after long press (" << longPressTime << " ms)"
             << endl;
      break;
    case btn.MultiPress:
      Serial << "Consecutive presses: " << btn.getCurrentMultiPressCount() + 1
             << endl;
      break;
    case btn.MultiPressDone:
      Serial << "Counted " << btn.getMultiPressCount() + 1
             << " consecutive presses" << endl;
      break;
  }
}
