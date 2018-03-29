#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "../Helpers/Linked_List.h"
#include "./DisplayHelpers.hpp"

class DisplayInterface
{
public:
  DisplayInterface()
  {
    LinkedList::append(this, first, last);
  }
  ~DisplayInterface() 
  {
    LinkedList::remove(this, first, last);
  }
  static DisplayInterface *getFirst()
  {
    return first;
  }
  DisplayInterface *getNext()
  {
    return next;
  }

  virtual void display() = 0;
  virtual void clearDisplay() = 0;

private:
  DisplayInterface *next;
  DisplayInterface *previous;

  static DisplayInterface *first;
  static DisplayInterface *last;

  template <class Node>
  friend void LinkedList::append(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::moveDown(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::remove(Node *, Node *&, Node *&);
};

DisplayInterface *DisplayInterface::first = nullptr;
DisplayInterface *DisplayInterface::last = nullptr;

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

#endif // DISPLAY_HPP