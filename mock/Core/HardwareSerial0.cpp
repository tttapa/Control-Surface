#include "ArduinoMock.hpp"
#include "HardwareSerial.h"

void HardwareSerial::begin(unsigned long baud) {
    ArduinoMock::getSerial().begin(baud);
}
void HardwareSerial::end() { ArduinoMock::getSerial().end(); }

int HardwareSerial::available() { return ArduinoMock::getSerial().available(); }
int HardwareSerial::read() { return ArduinoMock::getSerial().read(); }
int HardwareSerial::peek() { return ArduinoMock::getSerial().peek(); }

size_t HardwareSerial::write(uint8_t data) {
    return ArduinoMock::getSerial().write(data);
}
size_t HardwareSerial::write(const uint8_t *data, size_t len) {
    return ArduinoMock::getSerial().write(data, len);
}

HardwareSerial Serial;