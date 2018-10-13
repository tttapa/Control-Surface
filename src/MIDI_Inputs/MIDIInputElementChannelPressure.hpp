#pragma once

#include "MIDIInputElement.hpp"
#include <Helpers/LinkedList.hpp>

class MIDIInputElementChannelPressure
    : public MIDIInputElement,
      public DoublyLinkable<MIDIInputElementChannelPressure> {
  public:
    /**
     * @brief   Constructor.
     * @todo    Documentation.
     */
    MIDIInputElementChannelPressure(Channel channel)
        : MIDIInputElement({0, channel}) {
        elements.append(this);
    }

    /**
     * @brief   Destructor.
     * @todo    Documentation.
     */
    virtual ~MIDIInputElementChannelPressure() { elements.remove(this); }

    static void beginAll() {
        for (MIDIInputElementChannelPressure &el : elements)
            el.begin();
    }

    /**
     * @brief   Reset all MIDIInputElementChannelPressure elements to their 
     *          initial state.
     *
     * @see     MIDIInputElementNote#reset
     */
    static void resetAll() {
        for (MIDIInputElementChannelPressure &el : elements)
            el.reset();
    }

    /**
     * @brief   Update all MIDIInputElementChannelPressure elements.
     */
    static void updateAll() {
        for (MIDIInputElementChannelPressure &el : elements)
            el.update();
    }

    /**
     * @brief   Update all MIDIInputElementChannelPressure elements with a new MIDI
     *          message.
     *
     * @see     MIDIInputElementChannelPressure#updateWith
     */
    static void updateAllWith(const MIDI_message_matcher &midimsg) {
        for (MIDIInputElementChannelPressure &el : elements)
            if (el.updateWith(midimsg))
                return;
    }

  private:
    MIDICNChannelAddress
    getTarget(const MIDI_message_matcher &midimsg) const override {
        return {0, Channel(midimsg.channel)};
        // Channel Pressure doesn't have an address
    }

    void moveDown() override { elements.moveDown(this); }

    static DoublyLinkedList<MIDIInputElementChannelPressure> elements;
};
