#ifndef SHIFTREGISTEROUT_H_
#define SHIFTREGISTEROUT_H_

#include "Arduino.h"
#include "ExtendedIOElement.h"

class ShiftRegisterOut : public ExtendedIOElement
{
public:
  ShiftRegisterOut(pin_t dataPin, pin_t clockPin, pin_t latchPin, uint8_t bitOrder, pin_t length = 8);
  ~ShiftRegisterOut();

  void digitalWrite(pin_t pin, uint8_t val);
  int digitalRead(pin_t pin);
  void begin();
  void refresh();
  void reset();

  pin_t green(pin_t id);
  pin_t red(pin_t id);
  pin_t blue(pin_t id);

  uint8_t redBit = 0;
  uint8_t greenBit = 1;
  uint8_t blueBit = 2;

private:
  pin_t dataPin, clockPin, latchPin;
  uint8_t bitOrder, bufferLength;

  uint8_t *stateBuffer = nullptr;

  int8_t pinToBufferIndex(pin_t pin);
  uint8_t pinToBitMask(pin_t pin);
  const int8_t INVALID_PIN = -1;
};

#endif // SHIFTREGISTEROUT_H_