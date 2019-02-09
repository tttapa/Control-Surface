#pragma once

#include "MIDIInputElement.hpp"
#include <Helpers/LinkedList.hpp>

class MIDIInputElementPC : public MIDIInputElement,
                           public DoublyLinkable<MIDIInputElementPC> {
  public:
    /**
     * @brief   Constructor.
     * @todo    Documentation.
     */
    MIDIInputElementPC(const MIDICNChannelAddress &address)
        : MIDIInputElement(address) {
        elements.append(this);
    }

    /**
     * @brief   Destructor.
     * @todo    Documentation.
     */
    virtual ~MIDIInputElementPC() { elements.remove(this); }

    static void beginAll() {
        for (MIDIInputElementPC &el : elements)
            el.begin();
    }

    /**
     * @brief   Reset all MIDIInputElementPC elements to their 
     *          initial state.
     *
     * @see     MIDIInputElementPC#reset
     */
    static void resetAll() {
        for (MIDIInputElementPC &el : elements)
            el.reset();
    }

    /**
     * @brief   Update all MIDIInputElementPC elements.
     */
    static void updateAll() {
        for (MIDIInputElementPC &el : elements)
            el.update();
    }

    /**
     * @brief   Update all MIDIInputElementPC elements with a new MIDI
     *          message.
     *
     * @see     MIDIInputElementPC#updateWith
     */
    static void updateAllWith(const MIDI_message_matcher &midimsg) {
        for (MIDIInputElementPC &el : elements)
            if (el.updateWith(midimsg))
                return;
    }

  private:
    MIDICNChannelAddress
    getTarget(const MIDI_message_matcher &midimsg) const override {
        return {0, Channel(midimsg.channel), midimsg.CN};
        // Program Change doesn't have an address
    }

    void moveDown() override { elements.moveDown(this); }

    static DoublyLinkedList<MIDIInputElementPC> elements;
};