#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "../Helpers/Linked_List.h"
#include "./DisplayHelpers.hpp"

class Display
{
public:
  Display(Adafruit_GFX &display) : display(display)
  {
    LinkedList::append(this, first, last); // TODO: can be singly linked list
  }
  // No destructor (intentional memory leak)

  static void add(Adafruit_GFX &display)
  {
    for (Display *el = getFirst(); el != nullptr; el->getNext())
      if (&display == &(el->display))
        return;
    new Display(display);
  }
  static Display *getFirst()
  {
    return first;
  }
  Display *getNext()
  {
    return next;
  }

  Adafruit_GFX &display;

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
    Display::add(display);
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

protected:
  virtual void draw() = 0;
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