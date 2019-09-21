/* ✔ */

#pragma once

#include "ChannelMessageMatcher.hpp"
#include <Def/MIDICNChannelAddress.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class for objects that listen for incoming MIDI events.
 * 
 * They can either update some kind of display, or they can just save the state.
 */
class MIDIInputElement {
  protected:
    MIDIInputElement() {} // not used, only for virtual inheritance
    /**
     * @brief   Create a new MIDIInputElement that listens on the given address.
     * 
     * @param   address
     *          The MIDI address to listen to.
     */
    MIDIInputElement(const MIDICNChannelAddress &address) : address(address) {}

  public:
    virtual ~MIDIInputElement() = default;

    /// Initialize the input element.
    virtual void begin() {}

    /// Update the display of the input element.
    virtual void display() const {}

    /// Reset the input element to its initial state.
    virtual void reset() = 0;

    /// Update the value of the input element. Used for decaying VU meters etc.
    virtual void update() {}

    /// Receive a new MIDI message and update the internal state.
    bool updateWith(const ChannelMessageMatcher &midimsg) {
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
    /// Update the internal state with the new MIDI message.
    virtual bool updateImpl(const ChannelMessageMatcher &midimsg,
                            const MIDICNChannelAddress &target) = 0;

    /**
     * @brief   Extract the target address from a MIDI message.
     * @note    This base version of the function is only valid for messages 
     *          that use data1 as an address (i.e. Note On, Note Off, Polyphonic
     *          Key Pressure and Control Change).
     */
    virtual inline MIDICNChannelAddress
    getTarget(const ChannelMessageMatcher &midimsg) const {
        return {int8_t(midimsg.data1), Channel(midimsg.channel), midimsg.CN};
    }

    /**
     * @brief   Check if the address of the incoming MIDI message matches an 
     *          address of this element.
     * @note    This base version of the function is only valid for non-Bankable
     *          MIDI input elements.
     */
    virtual inline bool match(const MIDICNChannelAddress &target) const {
        return MIDICNChannelAddress::matchSingle(this->address, target);
    }

    /**
     * @brief   Move down this element in the linked list of elements.
     * 
     * This means that the element will be checked earlier on the next
     * iteration.
     */
    virtual void moveDown() = 0;

  protected:
    const MIDICNChannelAddress address;
};

END_CS_NAMESPACE