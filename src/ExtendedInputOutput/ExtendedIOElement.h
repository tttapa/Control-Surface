#ifndef EXTENDEDIOELEMENT_H_
#define EXTENDEDIOELEMENT_H_

#include "Arduino.h"

// #define DEBUG

typedef unsigned int pin_t;
typedef int analog_t;

class ExtendedIOElement
{
  public:
    ExtendedIOElement(){};
    ~ExtendedIOElement(){};

    virtual void pinMode(pin_t pin, uint8_t mode){};
    virtual void digitalWrite(pin_t pin, uint8_t mode){};
    virtual int digitalRead(pin_t pin){};
    virtual analog_t analogRead(pin_t pin){};
    virtual void analogWrite(pin_t pin, analog_t val){};

    virtual void begin(){};
    virtual void refresh(){};
    virtual void reset(){};

    pin_t pin(pin_t p)
    {
        return p + offset;
    }

    void setOffset(pin_t offset)
    {
        this->offset = offset;
    }

    pin_t getLastPin()
    {
        return offset + length;
    }

    pin_t getLength()
    {
        return length;
    }

  protected:
    pin_t length;
    uint8_t *stateBuffer = nullptr;
    analog_t *analogBuffer = nullptr;

    pin_t offset;

    int8_t pinToBufferIndex(pin_t pin)
    {
        if (pin >= length)
            return INVALID_PIN;
        return pin >> 3; // pin / 8;
    }
    uint8_t pinToBitMask(pin_t pin)
    {
        return 1 << (pin & 7); // 1 << (pin % 8);
    }
    const int8_t INVALID_PIN = -1;
};

#endif // EXTENDEDIOELEMENT_H_