#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "../Helpers/Linked_List.h"

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

#endif // DISPLAY_HPP