#pragma once

#include <Helpers/LinkedList.h>
#include <stdint.h>

/**
 * @brief   A base class for all MIDI output elements.
 *
 * All MIDI output elements are added to a linked list,
 * so they can be updated all at once by simply looping over
 * the entire list.
 */
class MIDIOutputElement {
  protected:
    MIDIOutputElement() { LinkedList::append(this, first, last); }

  public:
    virtual ~MIDIOutputElement() { LinkedList::remove(this, first, last); }

    virtual void update() = 0;

    static void updateAll() {
        for (MIDIOutputElement *el = first; el; el = el->next)
            el->update();
    }

  private:
    MIDIOutputElement *next = nullptr;
    MIDIOutputElement *previous = nullptr;
    static MIDIOutputElement *last;
    static MIDIOutputElement *first;

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};