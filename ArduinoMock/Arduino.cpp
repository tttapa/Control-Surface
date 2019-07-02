#include "Arduino.h"
#include "ArduinoMock.hpp"

void sei() {}
void cli() {}

void yield() {}

void pinMode(uint8_t pin, uint8_t mode) {
    ArduinoMock::getInstance().pinMode(pin, mode);
}

void digitalWrite(uint8_t pin, uint8_t state) {
    ArduinoMock::getInstance().digitalWrite(pin, state);
}

int digitalRead(uint8_t pin) {
    return ArduinoMock::getInstance().digitalRead(pin);
}

int analogRead(uint8_t pin) {
    return ArduinoMock::getInstance().analogRead(pin);
}

void analogWrite(uint8_t pin, int value) {
    ArduinoMock::getInstance().analogWrite(pin, value);
}

unsigned long millis() { return ArduinoMock::getInstance().millis(); }

unsigned long micros() { return ArduinoMock::getInstance().micros(); }