#pragma once

#include "../Helpers/LinkedList.h"
#include "MIDI_Input_Element_Addressable.hpp"

class MIDI_Input_CC : public MIDI_Input_Element_Addressable {
  public:
    /**
     * @brief   Constructor.
     * @todo    Documentation.
     */
    MIDI_Input_CC(uint8_t baseChannel, uint8_t baseAddress)
        : MIDI_Input_Element_Addressable(baseChannel, baseAddress) {
        LinkedList::append(this, first, last);
    }

    /**
     * @brief   Destructor.
     * @todo    Documentation.
     */
    virtual ~MIDI_Input_CC() { LinkedList::remove(this, first, last); }

    /**
     * @brief   Update all MIDI_Input_CC elements.
     */
    static void updateAll() {
        for (MIDI_Input_CC *e = first; e; e = e->next)
            e->update();
    }

    /** 
     * @brief   Reset all MIDI_Input_CC elements to their initial state.
     * 
     * @see     MIDI_Input_CC#reset
     */
    static void resetAll() {
        for (MIDI_Input_CC *e = first; e; e = e->next)
            e->reset();
    }

    /**
     * @brief   Update all MIDI_Input_CC elements with a new MIDI message.
     * 
     * @see     MIDI_Input_CC#updateWith
     */
    static void updateAllWith(const MIDI_message_matcher &midimsg) {
        for (MIDI_Input_CC *e = first; e; e = e->next)
            if (e->updateWith(midimsg))
                return;
    }

  private:
    void moveDown() override { LinkedList::moveDown(this, first, last); }

    MIDI_Input_CC *next = nullptr, *previous = nullptr;
    static MIDI_Input_CC *last;
    static MIDI_Input_CC *first;

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::moveDown(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};
