/**
 * This examples shows how use the debug macros for printing different kinds of
 * debugging information
 * 
 * @boards  AVR, AVR USB, Nano 33, Due, Teensy 3.x, ESP8266, ESP32
 * 
 * Behavior
 * --------
 * 
 * - Enable debugging in `src/AH/Settings/Settings.hpp`.
 * - Upload the sketch to the Arduino, and open the Serial Monitor 
 *   (`CTRL+Shift+M`)
 * - Every five seconds, debugging information is printed, for example:
 * 
 * ~~~
 * This is the result of `DEBUG`
 * [/home/pieter/GitHub/Arduino-Debugging/Example/Example.ino:12]:	This is the result of `DEBUGREF`
 * [void loop() @ line 13]:	This is the result of `DEBUGFN`
 * [0:2:11.085]:	This is the result of `DEBUGTIME`
 * a = 1, b = 2, c = 3
 * log10(1000) - 2 = 1.00
 * millis() = 131085
 * Serial.read() = -1
 * [int someFunction(int) @ line 26]:	parameter = 42
 * ~~~
 * 
 * Written by PieterP, 2018-07-31  
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Arduino_Helpers.h>

#include <AH/Debug/Debug.hpp>

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
}

void loop() {
  DEBUG("This is the result of `DEBUG`");
  DEBUGREF("This is the result of `DEBUGREF`");
  DEBUGFN("This is the result of `DEBUGFN`");
  DEBUGTIME("This is the result of `DEBUGTIME`");
  int a = 1, b = 2, c = 3;
  DEBUGVAL(a, b, c);
  DEBUGVAL(log10(1000) - 2);
  DEBUGVAL(millis());
  DEBUGVAL(Serial.read());
  someFunction(42);
  DEBUG("");
  delay(5000);
}

int someFunction(int parameter) {
  DEBUGFN(NAMEDVALUE(parameter));
  return parameter;
}