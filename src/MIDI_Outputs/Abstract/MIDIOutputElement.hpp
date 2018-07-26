#pragma once

#include <Helpers/LinkedList.h>
#include <stdint.h>

#include <Helpers/Debug.hpp>

/**
 * @brief   A base class for all MIDI output elements.
 *
 * All MIDI output elements are added to a linked list,
 * so they can be updated all at once by simply looping over
 * the entire list.
 */
class MIDIOutputElement {
  protected:
    MIDIOutputElement() { enable(); }

  public:
    virtual ~MIDIOutputElement() { disable(); }

    void enable() {
        if (LinkedList::contains(this, first, last)) {
            DEBUGFN(F("Error: This element is already in the linked list."));
            ERROR(return );
        }
        LinkedList::append(this, first, last);
    }

    static void enable(MIDIOutputElement *element) {
        element->enable();
    }

    static void enable(MIDIOutputElement &element) {
        element.enable();
    }

    template <class T, size_t N>
    static void enable(T (&array)[N]) {
        for (T &el : array)
            enable(el);
    }

    void disable() { LinkedList::remove(this, first, last); }

    static void disable(MIDIOutputElement *element) {
        element->disable();
    }

    static void disable(MIDIOutputElement &element) {
        element.disable();
    }

    template <class T, size_t N>
    static void disable(T (&array)[N]) {
        for (T &el : array)
            disable(el);
    }

    virtual void begin() = 0;

    static void beginAll() {
        for (MIDIOutputElement *el = first; el; el = el->next)
            el->begin();
    }

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
    friend bool LinkedList::contains(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};