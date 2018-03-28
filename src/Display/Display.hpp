#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "../Helpers/Linked_List.h"
#include "./DisplayHelpers.hpp"

class Display
{
public:
  Display()
  {
    LinkedList::append(this, first, last);
  }
  ~Display() 
  {
    LinkedList::remove(this, first, last);
  }
  static Display *getFirst()
  {
    return first;
  }
  Display *getNext()
  {
    return next;
  }

  virtual void display() = 0;
  virtual void clearDisplay() = 0;

private:
  Display *next;
  Display *previous;

  static Display *first;
  static Display *last;

  template <class Node>
  friend void LinkedList::append(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::moveDown(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::remove(Node *, Node *&, Node *&);
};

Display *Display::first = nullptr;
Display *Display::last = nullptr;

class DisplayElement
{
public:
  DisplayElement(Adafruit_GFX &display)
      : display(display)
  {
    LinkedList::append(this, first, last);
    // Display::add(display); // Doesn't work because Adafruit_GFX has no virtual clearDisplay and display functions
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