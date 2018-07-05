#pragma once

#include "../Helpers/LinkedList.h"
#include "../MIDI_Element.h"

/**
 * @brief   A base class for all MIDI Control elements.
 * 
 * All MIDI Control Elements are added to a linked list,
 * so they can be updated all at once by simply looping over
 * the entire list.
 */
class MIDI_Control_Element : public MIDI_Element {
    friend class Control_Surface_;

  public:
    MIDI_Control_Element() { LinkedList::append(this, first, last); }
    ~MIDI_Control_Element() { LinkedList::remove(this, first, last); }

    MIDI_Control_Element *getNext() { return next; } // @todo   Make private?
    static MIDI_Control_Element *getFirst() { return first; }

  private:
    MIDI_Control_Element *next = nullptr;
    MIDI_Control_Element *previous = nullptr;
    static MIDI_Control_Element *last;
    static MIDI_Control_Element *first;

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};