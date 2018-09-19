#pragma once

#include <Helpers/LinkedList.hpp>
#include <MIDI_Inputs/MIDIInputElement.hpp>

class MIDIInputElementCC : public MIDIInputElement,
                           public DoublyLinkable<MIDIInputElementCC> {
  public:
    /**
     * @brief   Constructor.
     * @todo    Documentation.
     */
    MIDIInputElementCC(uint8_t baseChannel, uint8_t baseAddress)
        : MIDIInputElement{{baseAddress, baseChannel}} {
        elements.append(this);
    }

    /**
     * @brief   Destructor.
     * @todo    Documentation.
     */
    virtual ~MIDIInputElementCC() { elements.remove(this); }

    /**
     * @brief   Initialize all MIDIInputElementCC elements.
     * 
     * @see     MIDIInputElementCC#begin
     */
    static void beginAll() {
        for (MIDIInputElementCC &e : elements)
            e.begin();
    }

    /**
     * @brief   Update all MIDIInputElementCC elements.
     * 
     * @see     MIDIInputElementCC#update
     */
    static void updateAll() {
        for (MIDIInputElementCC &e : elements)
            e.update();
    }

    /**
     * @brief   Reset all MIDIInputElementCC elements to their initial state.
     *
     * @see     MIDIInputElementCC#reset
     */
    static void resetAll() {
        for (MIDIInputElementCC &e : elements)
            e.reset();
    }

    /**
     * @brief   Update all MIDIInputElementCC elements with a new MIDI message.
     *
     * @see     MIDIInputElementCC#updateWith
     */
    static void updateAllWith(const MIDI_message_matcher &midimsg) {
        for (MIDIInputElementCC &e : elements)
            if (e.updateWith(midimsg))
                return;
    }

  private:
    void moveDown() override { elements.moveDown(this); }

    static DoublyLinkedList<MIDIInputElementCC> elements;
};
