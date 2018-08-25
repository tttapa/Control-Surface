#ifdef ARDUINO

#include "Error.hpp"
#include <Arduino.h>

void fatalErrorExit() {
#ifdef LED_BUILTIN
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
#endif
    cli();
    while (1)
        ;
}

#endif