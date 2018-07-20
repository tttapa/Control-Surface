#pragma once

#include "MIDI_Input_Element.hpp"
#include <Helpers/LinkedList.h>

class MIDI_Input_ChannelPressure : public MIDI_Input_Element {
  public:
    /**
     * @brief   Constructor.
     * @todo    Documentation.
     */
    MIDI_Input_ChannelPressure(uint8_t baseChannel)
        : MIDI_Input_Element(baseChannel) {
        LinkedList::append(this, first, last);
    }

    /**
     * @brief   Destructor.
     * @todo    Documentation.
     */
    virtual ~MIDI_Input_ChannelPressure() {
        LinkedList::remove(this, first, last);
    }

    /**
     * @brief   Reset all MIDI_Input_ChannelPressure elements to their initial
     *          state.
     *
     * @see     MIDI_Input_Note#reset
     */
    static void resetAll() {
        for (MIDI_Input_ChannelPressure *e = first; e; e = e->next)
            e->reset();
    }

    /**
     * @brief   Update all MIDI_Input_ChannelPressure elements.
     */
    static void updateAll() {
        for (MIDI_Input_ChannelPressure *e = first; e; e = e->next)
            e->update();
    }

    /**
     * @brief   Update all MIDI_Input_ChannelPressure elements with a new MIDI
     *          message.
     *
     * @see     MIDI_Input_ChannelPressure#updateWith
     */
    static void updateAllWith(const MIDI_message_matcher &midimsg) {
        for (MIDI_Input_ChannelPressure *e = first; e; e = e->next)
            if (e->updateWith(midimsg))
                return;
    }

  private:
    void moveDown() override { LinkedList::moveDown(this, first, last); }

    MIDI_Input_ChannelPressure *next = nullptr, *previous = nullptr;
    static MIDI_Input_ChannelPressure *last;
    static MIDI_Input_ChannelPressure *first;

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::moveDown(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};
