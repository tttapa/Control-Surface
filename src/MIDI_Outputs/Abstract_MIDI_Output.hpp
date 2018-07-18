#pragma once

#include <stdint.h>
#include <Helpers/LinkedList.h>

/**
 * @brief   A base class for all MIDI output elements.
 * 
 * All MIDI output elements are added to a linked list,
 * so they can be updated all at once by simply looping over
 * the entire list.
 */
class Abstract_MIDI_Output {
  public:
    Abstract_MIDI_Output() { LinkedList::append(this, first, last); }
    virtual ~Abstract_MIDI_Output() { LinkedList::remove(this, first, last); }

    virtual void update() = 0;

    static void updateAll() {
        for (Abstract_MIDI_Output *el = first; el; el = el->next)
            el->update();
    }

    virtual uint8_t getChannel(uint8_t baseChannel) const {
        return baseChannel;
    }

  private:
    Abstract_MIDI_Output *next = nullptr;
    Abstract_MIDI_Output *previous = nullptr;
    static Abstract_MIDI_Output *last;
    static Abstract_MIDI_Output *first;

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};