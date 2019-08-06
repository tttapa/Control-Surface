#pragma once

#include <Helpers/LinkedList.hpp>
#include <MIDI_Inputs/MIDIInputElement.hpp>

#if defined(ESP32)
#include <mutex>
#define GUARD_LIST_LOCK std::lock_guard<std::mutex> _guard(mutex)
#else
#define GUARD_LIST_LOCK
#endif

/**
 * @brief   Class for objects that listen for incoming MIDI Controller Change
 *          events.
 * 
 * All instances are added to a linked list that can be traversed to update
 * all of them when a new MIDI CC event is received.
 */
class MIDIInputElementCC : public MIDIInputElement,
                           public DoublyLinkable<MIDIInputElementCC> {
  public:
    /**
     * @brief   Create a new MIDIInputElementCC that listens on the given
     *          address.
     * 
     * Add the element to the linked list.
     * 
     * @param   address
     *          The MIDI address to listen to. (Controller number [0, 119],
     *          Channel [1, 16], Cable Number [0, 15].)
     */
    MIDIInputElementCC(const MIDICNChannelAddress &address)
        : MIDIInputElement{address} {
        GUARD_LIST_LOCK;
        elements.append(this);
    }

    /// Destructor: delete from the linked list.
    virtual ~MIDIInputElementCC() {
        GUARD_LIST_LOCK;
        elements.remove(this);
    }

    /// Initialize all MIDIInputElementCC elements.
    /// @see     MIDIInputElementCC#begin
    static void beginAll() {
        GUARD_LIST_LOCK;
        for (MIDIInputElementCC &e : elements)
            e.begin();
    }

    /// Update all MIDIInputElementCC elements.
    /// @see     MIDIInputElementCC#update
    static void updateAll() {
        GUARD_LIST_LOCK;
        for (MIDIInputElementCC &e : elements)
            e.update();
    }

    /// Reset all MIDIInputElementCC elements to their initial state.
    /// @see    MIDIInputElementCC::reset
    static void resetAll() {
        GUARD_LIST_LOCK;
        for (MIDIInputElementCC &e : elements)
            e.reset();
    }

    /// Update all MIDIInputElementCC elements with a new MIDI message.
    /// @see     MIDIInputElementCC#updateWith
    static void updateAllWith(const ChannelMessageMatcher &midimsg) {
        for (MIDIInputElementCC &e : elements)
            if (e.updateWith(midimsg))
                return;
        // No mutex required:
        // e.updateWith may alter the list, but if it does, it always returns
        // true, and we stop iterating, so it doesn't matter.
    }

  private:
    void moveDown() override {
        GUARD_LIST_LOCK;
        elements.moveDown(this);
    }

    static DoublyLinkedList<MIDIInputElementCC> elements;
#ifdef ESP32
    static std::mutex mutex;
#endif
};

#undef GUARD_LIST_LOCK