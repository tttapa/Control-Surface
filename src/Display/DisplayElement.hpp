#pragma once

#include <Display/DisplayInterface.hpp>
#include <Helpers/LinkedList.h>

struct Location {
    int16_t x;
    int16_t y;
};

class DisplayElement {
  protected:
    DisplayElement(DisplayInterface &display) : display(display) {
        LinkedList::append(this, first, last);
    }

  public:
    virtual ~DisplayElement() { LinkedList::remove(this, first, last); }

    virtual void draw() = 0;

    static void drawAll() {
        for (DisplayElement *el = first; el; el = el->next)
            el->draw();
    }

  protected:
    DisplayInterface &display;

    DisplayElement *next;
    DisplayElement *previous;

    static DisplayElement *first;
    static DisplayElement *last;

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};