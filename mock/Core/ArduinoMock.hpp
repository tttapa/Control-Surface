#pragma once

#include "HardwareSerial.h"

class ArduinoMock {
  private:
    ArduinoMock() {}
    SerialHelper serial;
    static ArduinoMock *instance;

  public:
    static ArduinoMock &getInstance();
    static void begin();
    static void end();
    static SerialHelper &getSerial();

    MOCK_METHOD(void, pinMode, (uint8_t, uint8_t));
    MOCK_METHOD(void, digitalWrite, (uint8_t, uint8_t));
    MOCK_METHOD(int, digitalRead, (uint8_t));
    MOCK_METHOD(int, analogRead, (uint8_t));
    MOCK_METHOD(void, analogReadResolution, (uint8_t));
    MOCK_METHOD(void, analogWrite, (uint8_t, int));
    MOCK_METHOD(void, shiftOut, (uint8_t, uint8_t, uint8_t, uint8_t));

    MOCK_METHOD(unsigned long, millis, ());
    MOCK_METHOD(unsigned long, micros, ());

    virtual ~ArduinoMock() = default;
};