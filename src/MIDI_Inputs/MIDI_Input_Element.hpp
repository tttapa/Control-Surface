#pragma once

#include "MIDI_message_matcher.h"

class MIDI_Input_Element {
  public:
    /**
     * @brief   Constructor.
     * @todo    Documentation.
     */
    MIDI_Input_Element(uint8_t baseChannel) : baseChannel(baseChannel) {}

    /**
     * @brief   Update the display of the input element.
     */
    virtual void display() {}

    /**
     * @brief   Update the value of the input element.  
     *          Used for decaying VU meters etc.
     */
    virtual void update() {}

    /**
     * @brief   Receive a new MIDI message and update the internal state.
     */
    bool updateWith(const MIDI_message_matcher &midimsg) {
        if (!matchChannel(midimsg.channel))
            return false;
        if (!updateImpl(midimsg))
            return false;
        moveDown();
        display();
    }

    inline uint8_t getBaseChannel() const { return baseChannel; }

  private:
    /**
     * @brief   Update the internal state with the new MIDI message.
     */
    virtual bool updateImpl(const MIDI_message_matcher &midimsg) = 0;

    /**
     * @brief   Check if the channel of the incoming MIDI message
     *          matches an channel of this element.
     */
    virtual inline bool matchChannel(uint8_t targetChannel) const {
        return getBaseChannel() == targetChannel;
    }

    /**
     * @brief   Move down this element in the linked list of elements.
     *          This means that the element will be checked earlier on the next
     *          iteration.
     */
    virtual void moveDown() = 0;

    const uint8_t baseChannel;
};
