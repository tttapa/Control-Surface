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

/**
 * @brief   Class for objects that listen for incoming MIDI Note events.
 * 
 * @ingroup MIDIInputElements
 */
class MIDIInputElementNote : public MIDIInputElement,
                             public DoublyLinkable<MIDIInputElementNote> {
  protected:
    /**
     * @brief   Constructor.
     * @todo    Documentation.
     */
    MIDIInputElementNote(const MIDICNChannelAddress &address)
        : MIDIInputElement{address} {
        GUARD_LIST_LOCK;
        elements.append(this);
    }

  public:
    /**
     * @brief   Destructor.
     * @todo    Documentation.
     */
    virtual ~MIDIInputElementNote() {
        GUARD_LIST_LOCK;
        elements.remove(this);
    }

    /**
     * @brief   Initialize all MIDIInputElementNote elements.
     * 
     * @see     MIDIInputElementNote#begin
     */
    static void beginAll() {
        GUARD_LIST_LOCK;
        for (MIDIInputElementNote &e : elements)
            e.begin();
    }

    /**
     * @brief   Update all MIDIInputElementNote elements.
     * 
     * @see     MIDIInputElementNote#update
     */
    static void updateAll() {
        GUARD_LIST_LOCK;
        for (MIDIInputElementNote &e : elements)
            e.update();
    }

    /** 
     * @brief   Reset all MIDIInputElementNote elements to their initial state.
     * 
     * @see     MIDIInputElementNote#reset
     */
    static void resetAll() {
        GUARD_LIST_LOCK;
        for (MIDIInputElementNote &e : elements)
            e.reset();
    }

    /**
     * @brief   Update all MIDIInputElementNote elements with a new MIDI 
     *          message.
     * 
     * @see     MIDIInputElementNote#updateWith
     */
    static void updateAllWith(const ChannelMessageMatcher &midimsg) {
        for (MIDIInputElementNote &e : elements)
            if (e.updateWith(midimsg)) {
                e.moveDown();
                return;
            }
        // No mutex required:
        // e.moveDown may alter the list, but if it does, it always returns,
        // and we stop iterating, so it doesn't matter.
    }

  private:
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

    static DoublyLinkedList<MIDIInputElementNote> elements;
#ifdef ESP32
    static std::mutex mutex;
#endif
};

#undef GUARD_LIST_LOCK

END_CS_NAMESPACE