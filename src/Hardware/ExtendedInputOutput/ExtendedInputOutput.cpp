#include "ExtendedInputOutput.h"
#include "ExtendedIOElement.h"
#include <Arduino.h>

namespace ExtIO {

ExtendedIOElement *getIOElementOfPin(pin_t pin) {
  for (ExtendedIOElement *el = ExtendedIOElement::getFirst(); el != nullptr;
       el = el->getNext())
    if (pin >= el->getStart() && pin < el->getEnd())
      return el;
  return nullptr;
}

void pinMode(pin_t pin, uint8_t mode) {
  if (pin < NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS)
    ::pinMode(pin, mode);
  else {
    ExtendedIOElement *el = getIOElementOfPin(pin);
    if (el != nullptr)
      el->pinMode(pin - el->getStart(), mode);
  }
}
void pinMode(int pin, uint8_t mode) { pinMode((pin_t)pin, mode); }

void digitalWrite(pin_t pin, uint8_t val) {
  if (pin < NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS)
    ::digitalWrite(pin, val);
  else {
    ExtendedIOElement *el = getIOElementOfPin(pin);
    if (el != nullptr)
      el->digitalWrite(pin - el->getStart(), val);
  }
}
void digitalWrite(int pin, uint8_t val) { digitalWrite((pin_t)pin, val); }

int digitalRead(pin_t pin) {
  if (pin < NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS)
    return ::digitalRead(pin);
  else {
    ExtendedIOElement *el = getIOElementOfPin(pin);
    if (el != nullptr)
      return el->digitalRead(pin - el->getStart());
  }
  return 0;
}
int digitalRead(int pin) { return digitalRead((pin_t)pin); }

void shiftOut(pin_t dataPin, pin_t clockPin, uint8_t bitOrder, uint8_t val) {
  uint8_t i;

  for (i = 0; i < 8; i++) {
    if (bitOrder == LSBFIRST)
      digitalWrite(dataPin, !!(val & (1 << i)));
    else
      digitalWrite(dataPin, !!(val & (1 << (7 - i))));

    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
  }
}
void shiftOut(int dataPin, int clockPin, uint8_t bitOrder, uint8_t val) {
  shiftOut((pin_t)dataPin, (pin_t)clockPin, bitOrder, val);
}

analog_t analogRead(pin_t pin) {
  if (pin < NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS)
    return ::analogRead(pin);
  else {
    ExtendedIOElement *el = getIOElementOfPin(pin);
    if (el != nullptr)
      return el->analogRead(pin - el->getStart());
  }
  return 0;
}
analog_t analogRead(int pin) { return analogRead((pin_t)pin); }

}; // namespace ExtIO