#pragma once

#include "../Helpers/LinkedList.h"
#include "MIDIInputElementAddressable.hpp"

class MIDIInputElementNote : public MIDIInputElementAddressable {
  public:
    /**
     * @brief   Constructor.
     * @todo    Documentation.
     */
    MIDIInputElementNote(uint8_t baseChannel, uint8_t baseAddress)
        : MIDIInputElementAddressable(baseChannel, baseAddress) {
        LinkedList::append(this, first, last);
    }

    /**
     * @brief   Destructor.
     * @todo    Documentation.
     */
    virtual ~MIDIInputElementNote() { LinkedList::remove(this, first, last); }

    /**
     * @brief   Update all MIDIInputElementNote elements.
     * 
     * @see     MIDIInputElementNote#update
     */
    static void updateAll() {
        for (MIDIInputElementNote *e = first; e; e = e->next)
            e->update();
    }

    /** 
     * @brief   Reset all MIDIInputElementNote elements to their initial state.
     * 
     * @see     MIDIInputElementNote#reset
     */
    static void resetAll() {
        for (MIDIInputElementNote *e = first; e; e = e->next)
            e->reset();
    }

    /**
     * @brief   Update all MIDIInputElementNote elements with a new MIDI message.
     * 
     * @see     MIDIInputElementNote#updateWith
     */
    static void updateAllWith(const MIDI_message_matcher &midimsg) {
        for (MIDIInputElementNote *e = first; e; e = e->next)
            if (e->updateWith(midimsg))
                return;
    }

  private:
    void moveDown() override { LinkedList::moveDown(this, first, last); }

    MIDIInputElementNote *next = nullptr, *previous = nullptr;
    static MIDIInputElementNote *last;
    static MIDIInputElementNote *first;

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::moveDown(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};