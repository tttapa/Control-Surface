#ifdef ARDUINO

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include "Error.hpp"

BEGIN_AH_NAMESPACE

void fatalErrorExit() {
#if defined(LED_BUILTIN) || (defined(ESP32) && defined(BUILTIN_LED))
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
#warning "LED_BUILTIN is not available, so it cannot blink when an error occurs"
    noInterrupts();
    while (1)
        yield();
#endif
}

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()

#endif