#include <Arduino.h>

void exit(int status) {
#ifdef LED_BUILTIN
  if (status != 0) {
    pinMode(LED_BUILTIN, OUTPUT);
    while (1) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(50);
      digitalWrite(LED_BUILTIN, LOW);
      delay(50);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(50);
      digitalWrite(LED_BUILTIN, LOW);
      delay(850);
    }
  }
#endif
  cli();
  while (1);
}