#pragma once

#include "MIDIInputElement.hpp"
#include <Helpers/LinkedList.hpp>

#if defined(ESP32)
#include <mutex>
#define GUARD_LIST_LOCK std::lock_guard<std::mutex> _guard(mutex)
#else
#define GUARD_LIST_LOCK
#endif

class MIDIInputElementChannelPressure
    : public MIDIInputElement,
      public DoublyLinkable<MIDIInputElementChannelPressure> {
  public:
    /**
     * @brief   Constructor.
     * @todo    Documentation.
     */
    MIDIInputElementChannelPressure(const MIDICNChannelAddress &address)
        : MIDIInputElement(address) {
        GUARD_LIST_LOCK;
        elements.append(this);
    }

    /**
     * @brief   Destructor.
     * @todo    Documentation.
     */
    virtual ~MIDIInputElementChannelPressure() {
        GUARD_LIST_LOCK;
        elements.remove(this);
    }

    static void beginAll() {
        GUARD_LIST_LOCK;
        for (MIDIInputElementChannelPressure &el : elements)
            el.begin();
    }

    /**
     * @brief   Reset all MIDIInputElementChannelPressure elements to their 
     *          initial state.
     *
     * @see     MIDIInputElementChannelPressure#reset
     */
    static void resetAll() {
        GUARD_LIST_LOCK;
        for (MIDIInputElementChannelPressure &el : elements)
            el.reset();
    }

    /**
     * @brief   Update all MIDIInputElementChannelPressure elements.
     */
    static void updateAll() {
        GUARD_LIST_LOCK;
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
        // No mutex required:
        // e.updateWith may alter the list, but if it does, it always returns
        // true, and we stop iterating, so it doesn't matter.
    }

  private:
    MIDICNChannelAddress
    getTarget(const MIDI_message_matcher &midimsg) const override {
        return {0, Channel(midimsg.channel), midimsg.CN};
        // Channel Pressure doesn't have an address
    }

    void moveDown() override {
        GUARD_LIST_LOCK;
        elements.moveDown(this);
    }

    static DoublyLinkedList<MIDIInputElementChannelPressure> elements;
#ifdef ESP32
    static std::mutex mutex;
#endif
};

#undef GUARD_LIST_LOCK