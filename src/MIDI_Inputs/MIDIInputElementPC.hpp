#pragma once

#include "MIDIInputElement.hpp"
#include <AH/Containers/LinkedList.hpp>

#if defined(ESP32)
#include <mutex>
#define GUARD_LIST_LOCK std::lock_guard<std::mutex> _guard(mutex)
#else
#define GUARD_LIST_LOCK
#endif

BEGIN_CS_NAMESPACE

class MIDIInputElementPC : public MIDIInputElement,
                           public DoublyLinkable<MIDIInputElementPC> {
  public:
    /**
     * @brief   Constructor.
     * @todo    Documentation.
     */
    MIDIInputElementPC(const MIDICNChannelAddress &address)
        : MIDIInputElement(address) {
        GUARD_LIST_LOCK;
        elements.append(this);
    }

    /**
     * @brief   Destructor.
     * @todo    Documentation.
     */
    virtual ~MIDIInputElementPC() {
        GUARD_LIST_LOCK;
        elements.remove(this);
    }

    static void beginAll() {
        GUARD_LIST_LOCK;
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
        GUARD_LIST_LOCK;
        for (MIDIInputElementPC &el : elements)
            el.reset();
    }

    /**
     * @brief   Update all MIDIInputElementPC elements.
     */
    static void updateAll() {
        GUARD_LIST_LOCK;
        for (MIDIInputElementPC &el : elements)
            el.update();
    }

    /**
     * @brief   Update all MIDIInputElementPC elements with a new MIDI
     *          message.
     *
     * @see     MIDIInputElementPC#updateWith
     */
    static void updateAllWith(const ChannelMessageMatcher &midimsg) {
        for (MIDIInputElementPC &e : elements)
            if (e.updateWith(midimsg)) {
                e.moveDown();
                return;
            }
        // No mutex required:
        // e.moveDown may alter the list, but if it does, it always returns,
        // and we stop iterating, so it doesn't matter.
    }

  private:
    /// Program Change doesn't have an address, so the target consists of just
    /// the channel and the cable number.
    MIDICNChannelAddress
    getTarget(const ChannelMessageMatcher &midimsg) const override {
        return {0, Channel(midimsg.channel), midimsg.CN};
    }

    /**
     * @brief   Move down this element in the linked list of elements.
     * 
     * This means that the element will be checked earlier on the next
     * iteration.
     */
    void moveDown() {
        GUARD_LIST_LOCK;
        elements.moveDown(this);
    }

    static DoublyLinkedList<MIDIInputElementPC> elements;
#ifdef ESP32
    static std::mutex mutex;
#endif
};

#undef GUARD_LIST_LOCK

END_CS_NAMESPACE