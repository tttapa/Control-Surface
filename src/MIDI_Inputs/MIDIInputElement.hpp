#pragma once

#include "MIDI_message_matcher.h"
#include <Def/MIDICNChannelAddress.hpp>

class MIDIInputElement {
  protected:
    /**
     * @brief   Constructor.
     * @todo    Documentation.
     */
    MIDIInputElement(const MIDICNChannelAddress &address) : address(address) {}

  public:
    virtual ~MIDIInputElement() = default;

    /**
     * @brief   Initialize the input element.
     */
    virtual void begin() {}

    /**
     * @brief   Update the display of the input element.
     */
    virtual void display() const {}

    /** 
     * @brief   Reset the input element to its initial state.
     */
    virtual void reset() = 0;

    /**
     * @brief   Update the value of the input element.  
     *          Used for decaying VU meters etc.
     */
    virtual void update() {}

    /**
     * @brief   Receive a new MIDI message and update the internal state.
     */
    bool updateWith(const MIDI_message_matcher &midimsg) {
        DEBUGFN("");
        MIDICNChannelAddress target = getTarget(midimsg);
        if (!this->match(target))
            return false;
        DEBUGFN(F("MIDI message matches"));
        if (!updateImpl(midimsg, target))
            return false;
        DEBUGFN(F("Updated"));
        moveDown();
        display();
        return true;
    }

  private:
    /**
     * @brief   Update the internal state with the new MIDI message.
     */
    virtual bool updateImpl(const MIDI_message_matcher &midimsg,
                            const MIDICNChannelAddress &target) = 0;

    /**
     * @brief   Extract the target address from a MIDI message.
     * @note    This base version of the function is only valid for messages 
     *          that use data1 as an address (i.e. Note On, Note Off, Polyphonic
     *          Key Pressure and Control Change).
     */
    virtual inline MIDICNChannelAddress
    getTarget(const MIDI_message_matcher &midimsg) const {
        return {int8_t(midimsg.data1), Channel(midimsg.channel)};
    }

    /**
     * @brief   Check if the address and channel of the incoming MIDI message
     *          match an address and channel of this element.
     * @note    This base version of the function is only valid for non-Bankable
     *          MIDI input elements.
     */
    virtual inline bool match(const MIDICNChannelAddress &target) const {
        return this->address == target;
    }

    /**
     * @brief   Move down this element in the linked list of elements.
     *          This means that the element will be checked earlier on the next
     *          iteration.
     */
    virtual void moveDown() = 0;

  protected:
    const MIDICNChannelAddress address;
};
