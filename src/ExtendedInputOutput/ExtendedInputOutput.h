#ifndef EXTENDEDINPUTOUTPUT_H_
#define EXTENDEDINPUTOUTPUT_H_

#include "Arduino.h"
#include "ExtendedIOElement.h"

#define EXT_PIN(x) (x + NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS)

namespace ExtIO
{
extern ExtendedIOElement *getIOElementOfPin(pin_t pin);
extern void pinMode(pin_t pin, uint8_t mode);
extern void pinMode(int pin, uint8_t mode);
extern void digitalWrite(pin_t pin, uint8_t val);
extern void digitalWrite(int pin, uint8_t val);
extern int digitalRead(pin_t pin);
extern int digitalRead(int pin);
extern void shiftOut(pin_t dataPin, pin_t clockPin, uint8_t bitOrder, uint8_t val);
extern void shiftOut(int dataPin, int clockPin, uint8_t bitOrder, uint8_t val);
extern analog_t analogRead(pin_t pin);
extern analog_t analogRead(int pin);
}

#endif // EXTENDEDINPUTOUTPUT_H_