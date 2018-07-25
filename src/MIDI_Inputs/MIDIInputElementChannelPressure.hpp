#pragma once

#include "MIDIInputElement.hpp"
#include <Helpers/LinkedList.h>

class MIDIInputElementChannelPressure : public MIDIInputElement {
  public:
    /**
     * @brief   Constructor.
     * @todo    Documentation.
     */
    MIDIInputElementChannelPressure(uint8_t baseChannel)
        : MIDIInputElement(baseChannel) {
        LinkedList::append(this, first, last);
    }

    /**
     * @brief   Destructor.
     * @todo    Documentation.
     */
    virtual ~MIDIInputElementChannelPressure() {
        LinkedList::remove(this, first, last);
    }

    static void beginAll() {
        for (MIDIInputElementChannelPressure *el = first; el; el = el->next)
            el->begin();
    }

    /**
     * @brief   Reset all MIDIInputElementChannelPressure elements to their initial
     *          state.
     *
     * @see     MIDIInputElementNote#reset
     */
    static void resetAll() {
        for (MIDIInputElementChannelPressure *e = first; e; e = e->next)
            e->reset();
    }

    /**
     * @brief   Update all MIDIInputElementChannelPressure elements.
     */
    static void updateAll() {
        for (MIDIInputElementChannelPressure *e = first; e; e = e->next)
            e->update();
    }

    /**
     * @brief   Update all MIDIInputElementChannelPressure elements with a new MIDI
     *          message.
     *
     * @see     MIDIInputElementChannelPressure#updateWith
     */
    static void updateAllWith(const MIDI_message_matcher &midimsg) {
        for (MIDIInputElementChannelPressure *e = first; e; e = e->next)
            if (e->updateWith(midimsg))
                return;
    }

  private:
    void moveDown() override { LinkedList::moveDown(this, first, last); }

    MIDIInputElementChannelPressure *next = nullptr, *previous = nullptr;
    static MIDIInputElementChannelPressure *last;
    static MIDIInputElementChannelPressure *first;

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::moveDown(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};
