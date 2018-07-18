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
class MIDI_Control_Element {

  public:
    MIDI_Control_Element() { LinkedList::append(this, first, last); }
    virtual ~MIDI_Control_Element() { LinkedList::remove(this, first, last); }

    virtual void update() = 0;
    
    void updateAll() {
        for (MIDI_Control_Element *el = first; el; el = el->next)
            el->update();
    }

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