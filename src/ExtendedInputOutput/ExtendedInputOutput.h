#ifndef EXTENDEDINPUTOUTPUT_H_
#define EXTENDEDINPUTOUTPUT_H_

#include "Arduino.h"
#include "ExtendedIOElement.h"

#define EXT_PIN(x) (x + NUM_DIGITAL_PINS)

struct ExtendedIOListNode
{
    ExtendedIOListNode *next;
    pin_t start;
    pin_t end;
    ExtendedIOElement *element = nullptr;
};

class ExtendedInputOutput
{
  public:
    void pinMode(pin_t pin, uint8_t mode);
    void digitalWrite(pin_t pin, uint8_t val);
    int digitalRead(pin_t pin);

    void add(ExtendedIOElement &IOel);

    // private:
    pin_t pinOffset = NUM_DIGITAL_PINS;

    ExtendedIOListNode *firstExtIOLN = nullptr;
    ExtendedIOListNode *lastExtIOLN = nullptr;
};

extern ExtendedInputOutput ExtendedIO;

typedef unsigned int pin_t;
typedef int analog_t;

namespace ExtIO
{
extern void pinMode(pin_t pin, uint8_t mode);
extern void digitalWrite(pin_t pin, uint8_t val);
extern void shiftOut(pin_t dataPin, pin_t clockPin, uint8_t bitOrder, uint8_t val);
}

#endif // EXTENDEDINPUTOUTPUT_H_