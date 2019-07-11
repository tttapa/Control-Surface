#ifdef ARDUINO

#include "Error.hpp"
#include <Arduino.h>

void fatalErrorExit() {
#if defined(LED_BUILTIN) || defined(ESP32)
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
#else
    noInterrupts();
    while (1)
        yield();
#endif
}

#endif