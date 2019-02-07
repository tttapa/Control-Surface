#pragma once

#include "MIDIInputElement.hpp"
#include <Helpers/LinkedList.hpp>

/**
 * @brief   Class for objects that listen for incoming MIDI Note events.
 * 
 * @ingroup MIDIInputElements
 */
class MIDIInputElementNote : public MIDIInputElement,
                             public DoublyLinkable<MIDIInputElementNote> {
  public:
    /**
     * @brief   Constructor.
     * @todo    Documentation.
     */
    MIDIInputElementNote(const MIDICNChannelAddress &address)
        : MIDIInputElement{address} {
        elements.append(this);
    }

    /**
     * @brief   Destructor.
     * @todo    Documentation.
     */
    virtual ~MIDIInputElementNote() { elements.remove(this); }

    /**
     * @brief   Initialize all MIDIInputElementNote elements.
     * 
     * @see     MIDIInputElementNote#begin
     */
    static void beginAll() {
        for (MIDIInputElementNote &e : elements)
            e.begin();
    }

    /**
     * @brief   Update all MIDIInputElementNote elements.
     * 
     * @see     MIDIInputElementNote#update
     */
    static void updateAll() {
        for (MIDIInputElementNote &e : elements)
            e.update();
    }

    /** 
     * @brief   Reset all MIDIInputElementNote elements to their initial state.
     * 
     * @see     MIDIInputElementNote#reset
     */
    static void resetAll() {
        for (MIDIInputElementNote &e : elements)
            e.reset();
    }

    /**
     * @brief   Update all MIDIInputElementNote elements with a new MIDI 
     *          message.
     * 
     * @see     MIDIInputElementNote#updateWith
     */
    static void updateAllWith(const MIDI_message_matcher &midimsg) {
        for (MIDIInputElementNote &e : elements)
            if (e.updateWith(midimsg))
                return;
    }

  private:
    void moveDown() override { elements.moveDown(this); }

    static DoublyLinkedList<MIDIInputElementNote> elements;
};