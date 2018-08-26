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

    MOCK_METHOD2(pinMode, void(uint8_t, uint8_t));
    MOCK_METHOD2(digitalWrite, void(uint8_t, uint8_t));
    MOCK_METHOD1(digitalRead, int(uint8_t));
    MOCK_METHOD1(analogRead, int(uint8_t));
    MOCK_METHOD2(analogWrite, void(uint8_t, int));

    MOCK_METHOD0(millis, unsigned long());
    MOCK_METHOD0(micros, unsigned long());

    virtual ~ArduinoMock() = default;
};