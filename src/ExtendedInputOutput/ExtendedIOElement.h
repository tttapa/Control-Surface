#ifndef EXTENDEDIOELEMENT_H_
#define EXTENDEDIOELEMENT_H_

#include <stdint.h> // uint8_t 
#include "../Helpers/LinkedList.h"

typedef unsigned int pin_t;
typedef int analog_t;

class ExtendedIOElement
{
public:
  ExtendedIOElement(pin_t length)
      : length(length)
  {
    LinkedList::append(this, first, last);

    start = offset;
    end = offset + length;
    offset = end;
  }
  ~ExtendedIOElement()
  {
    LinkedList::remove(this, first, last);
  }

  virtual void pinMode(pin_t pin, uint8_t mode){}
  virtual void digitalWrite(pin_t pin, uint8_t mode){}
  virtual int digitalRead(pin_t pin){return 0;} // TODO: should these be pure virtual?
  virtual analog_t analogRead(pin_t pin){return 0;}
  virtual void analogWrite(pin_t pin, analog_t val){}

  virtual void begin() = 0;
  virtual void update() = 0;
  virtual void reset() = 0;

  pin_t pin(pin_t p);
  pin_t getEnd();
  pin_t getStart();

  ExtendedIOElement *getNext();
  static ExtendedIOElement *getFirst();

protected:
  pin_t length, start, end;
  static pin_t offset;

  ExtendedIOElement *next = nullptr, *previous = nullptr;
  static ExtendedIOElement *last;
  static ExtendedIOElement *first;
  template <class Node>
  friend void LinkedList::append(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::moveDown(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::remove(Node *, Node *&, Node *&);
};

#endif // EXTENDEDIOELEMENT_H_