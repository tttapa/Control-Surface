#pragma once

#include <Helpers/LinkedList.hpp>
#include <MIDI_Inputs/MIDIInputElement.hpp>

/**
 * @brief   Class for objects that listen for incoming MIDI Controller Change
 *          events.
 * 
 * All instances are added to a linked list that can be traversed to update
 * all of them when a new MIDI CC event is received.
 */
class MIDIInputElementCC : public MIDIInputElement,
                           public DoublyLinkable<MIDIInputElementCC> {
  public:
    /**
     * @brief   Create a new MIDIInputElementCC that listens on the given
     *          address.
     * 
     * Add the element to the linked list.
     * 
     * @param   address
     *          The MIDI address to listen to. (Controller number [0, 119],
     *          Channel [1, 16], Cable Number [0, 15].)
     */
    MIDIInputElementCC(const MIDICNChannelAddress &address)
        : MIDIInputElement{address} {
        elements.append(this);
    }

    /// Destructor: delete from the linked list.
    virtual ~MIDIInputElementCC() { elements.remove(this); }

    /// Initialize all MIDIInputElementCC elements.
    /// @see     MIDIInputElementCC#begin
    static void beginAll() {
        for (MIDIInputElementCC &e : elements)
            e.begin();
    }

    /// Update all MIDIInputElementCC elements.
    /// @see     MIDIInputElementCC#update
    static void updateAll() {
        for (MIDIInputElementCC &e : elements)
            e.update();
    }

    /// Reset all MIDIInputElementCC elements to their initial state.
    /// @see    MIDIInputElementCC::reset
    static void resetAll() {
        for (MIDIInputElementCC &e : elements)
            e.reset();
    }

    /// Update all MIDIInputElementCC elements with a new MIDI message.
    /// @see     MIDIInputElementCC#updateWith
    static void updateAllWith(const MIDI_message_matcher &midimsg) {
        for (MIDIInputElementCC &e : elements)
            if (e.updateWith(midimsg))
                return;
    }

  private:
    void moveDown() override { elements.moveDown(this); }

    static DoublyLinkedList<MIDIInputElementCC> elements;
};
