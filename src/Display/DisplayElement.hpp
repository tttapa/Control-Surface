#ifndef DISPLAYELEMENTS_HPP
#define DISPLAYELEMENTS_HPP

#include "./DisplayHelpers.hpp"
#include "../Helpers/LinkedList.h"

struct Location 
{
  int16_t x;
  int16_t y;
};

class DisplayElement
{
public:
  DisplayElement(Adafruit_GFX &display)
      : display(display)
  {
    LinkedList::append(this, first, last);
  }
  ~DisplayElement()
  {
    LinkedList::remove(this, first, last);
  }

  static DisplayElement *getFirst()
  {
    return first;
  }
  DisplayElement *getNext()
  {
    return next;
  }
  virtual void draw() = 0;

protected:
  Adafruit_GFX &display;

  DisplayElement *next;
  DisplayElement *previous;

  static DisplayElement *first;
  static DisplayElement *last;

  template <class Node>
  friend void LinkedList::append(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::moveDown(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::remove(Node *, Node *&, Node *&);
};

DisplayElement *DisplayElement::first = nullptr;
DisplayElement *DisplayElement::last = nullptr;

#endif // DISPLAYELEMENTS_HPP