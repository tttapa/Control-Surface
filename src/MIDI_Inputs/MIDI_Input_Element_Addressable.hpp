#pragma once

#include "MIDI_message_matcher.h"

class MIDI_Input_Element_Addressable {
  protected:
    /**
     * @brief   Constructor.
     * @todo    Documentation.
     */
    MIDI_Input_Element_Addressable(uint8_t baseChannel, uint8_t baseAddress)
        : baseChannel(baseChannel - 1), baseAddress(baseAddress) {}

  public:
    virtual ~MIDI_Input_Element_Addressable() = default;

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
        if (!match(midimsg))
            return false;
        DEBUGFN(F("MIDI message matches"));
        if (!updateImpl(midimsg))
            return false;
        DEBUGFN(F("Updated"));
        moveDown();
        display();
        return true;
    }

    inline uint8_t getBaseChannel() const { return baseChannel; }
    inline uint8_t getBaseAddress() const { return baseAddress; }

  private:
    /**
     * @brief   Update the internal state with the new MIDI message.
     */
    virtual bool updateImpl(const MIDI_message_matcher &midimsg) = 0;

    /**
     * @brief   Check if the address and channel of the incoming MIDI message
     *          match an address and channel of this element.
     */
    inline bool match(const MIDI_message_matcher &midimsg) const {
        return matchAddress(midimsg.data1) && matchChannel(midimsg.channel);
    }

    /**
     * @brief   Check if the address of the incoming MIDI message
     *          matches an address of this element.
     */
    virtual inline bool matchAddress(uint8_t targetAddress) const {
        DEBUGFN(F("target address = ")
                << hex << targetAddress << F("base address = ")
                << getBaseAddress() << dec);
        return getBaseAddress() == targetAddress;
    }

    /**
     * @brief   Check if the channel of the incoming MIDI message
     *          matches an channel of this element.
     */
    virtual inline bool matchChannel(uint8_t targetChannel) const {
        DEBUGFN(F("target channel = ")
                << hex << targetChannel << F("base channel = ")
                << getBaseChannel() << dec);
        return getBaseChannel() == targetChannel;
    }

    /**
     * @brief   Move down this element in the linked list of elements.
     *          This means that the element will be checked earlier on the next
     *          iteration.
     */
    virtual void moveDown() = 0;

    const uint8_t baseChannel;
    const uint8_t baseAddress;
};
