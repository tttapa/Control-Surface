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
class AbstractMIDIOutput {
  protected:
    AbstractMIDIOutput() { LinkedList::append(this, first, last); }

  public:
    virtual ~AbstractMIDIOutput() { LinkedList::remove(this, first, last); }

    virtual void update() = 0;

    static void updateAll() {
        for (AbstractMIDIOutput *el = first; el; el = el->next)
            el->update();
    }

  private:
    AbstractMIDIOutput *next = nullptr;
    AbstractMIDIOutput *previous = nullptr;
    static AbstractMIDIOutput *last;
    static AbstractMIDIOutput *first;

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};