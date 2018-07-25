#pragma once

#include <Helpers/LinkedList.h>
#include <MIDI_Inputs/MIDIInputElementAddressable.hpp>

class MIDIInputElementCC : public MIDIInputElementAddressable {
  public:
    /**
     * @brief   Constructor.
     * @todo    Documentation.
     */
    MIDIInputElementCC(uint8_t baseChannel, uint8_t baseAddress)
        : MIDIInputElementAddressable(baseChannel, baseAddress) {
        LinkedList::append(this, first, last);
    }

    /**
     * @brief   Destructor.
     * @todo    Documentation.
     */
    virtual ~MIDIInputElementCC() { LinkedList::remove(this, first, last); }

    static void beginAll() {
        for (MIDIInputElementCC *el = first; el; el = el->next)
            el->begin();
    }

    /**
     * @brief   Update all MIDIInputElementCC elements.
     */
    static void updateAll() {
        for (MIDIInputElementCC *e = first; e; e = e->next)
            e->update();
    }

    /**
     * @brief   Reset all MIDIInputElementCC elements to their initial state.
     *
     * @see     MIDIInputElementCC#reset
     */
    static void resetAll() {
        for (MIDIInputElementCC *e = first; e; e = e->next)
            e->reset();
    }

    /**
     * @brief   Update all MIDIInputElementCC elements with a new MIDI message.
     *
     * @see     MIDIInputElementCC#updateWith
     */
    static void updateAllWith(const MIDI_message_matcher &midimsg) {
        for (MIDIInputElementCC *e = first; e; e = e->next)
            if (e->updateWith(midimsg))
                return;
    }

  private:
    void moveDown() override { LinkedList::moveDown(this, first, last); }

    MIDIInputElementCC *next = nullptr, *previous = nullptr;
    static MIDIInputElementCC *last;
    static MIDIInputElementCC *first;

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::moveDown(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};
