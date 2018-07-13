#pragma once

#include "../Helpers/LinkedList.h"
#include "MIDI_Input_Element_Addressable.hpp"

class MIDI_Input_Note : public MIDI_Input_Element_Addressable {
  public:
    /**
     * @brief   Constructor.
     * @todo    Documentation.
     */
    MIDI_Input_Note(uint8_t baseChannel, uint8_t baseAddress)
        : MIDI_Input_Element_Addressable(baseChannel, baseAddress) {
        LinkedList::append(this, first, last);
    }

    /**
     * @brief   Destructor.
     * @todo    Documentation.
     */
    virtual ~MIDI_Input_Note() { LinkedList::remove(this, first, last); }

    /**
     * @brief   Update all MIDI_Input_Note elements.
     */
    static void updateAll() {
        for (MIDI_Input_Note *e = first; e; e = e->next)
            e->update();
    }

    /**
     * @brief   Update all MIDI_Input_Note elements with a new MIDI message.
     * 
     * @see     MIDI_Input_Note#updateWith
     */
    static void updateAllWith(const MIDI_message_matcher &midimsg) {
        for (MIDI_Input_Note *e = first; e; e = e->next)
            if (e->updateWith(midimsg))
                return;
    }

  private:
    void moveDown() override { LinkedList::moveDown(this, first, last); }

    MIDI_Input_Note *next = nullptr, *previous = nullptr;
    static MIDI_Input_Note *last;
    static MIDI_Input_Note *first;

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::moveDown(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};