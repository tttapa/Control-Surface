#ifndef EXTENDEDIOELEMENT_H_
#define EXTENDEDIOELEMENT_H_

#include "Arduino.h"

typedef unsigned int pin_t;
typedef int analog_t;

class ExtendedIOElement
{
  public:
    ExtendedIOElement(pin_t length)
      : length(length)
    {
      if (first == nullptr)
        first = this;
      previous = last;
      if (previous != nullptr)
        previous->next = this;
      last = this;
      next = nullptr;

      start = offset;
      end = offset + length;
      offset = end;
    };
    ~ExtendedIOElement() {
      if (previous != nullptr) {
        previous->next = next;
      }
      if (this == last) {
        last = previous;
      }
      if (next != nullptr) {
        next->previous = previous;
      }
      if (this == first) {
        first = next;
      }
    };

    virtual void pinMode(pin_t pin, uint8_t mode) {};
    virtual void digitalWrite(pin_t pin, uint8_t mode) {};
    virtual int digitalRead(pin_t pin) {};
    virtual analog_t analogRead(pin_t pin) {};
    virtual void analogWrite(pin_t pin, analog_t val) {};

    virtual void begin() {};
    virtual void refresh() {};
    virtual void reset() {};
    virtual void print() {};

    pin_t pin(pin_t p)
    {
      return p + start;
    }

    pin_t getEnd()
    {
      return end;
    }

    pin_t getStart() {
      return start;
    }

    ExtendedIOElement *getNext() {
      return next;
    }
    static ExtendedIOElement *getFirst() {
      return first;
    }


  protected:
    pin_t length, start, end;

    ExtendedIOElement *next = nullptr, *previous = nullptr;

    static pin_t offset;
    static ExtendedIOElement *last;
    static ExtendedIOElement *first;
};

#endif // EXTENDEDIOELEMENT_H_