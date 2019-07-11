#pragma once

#include "MIDIInputElement.hpp"
#include <Helpers/LinkedList.hpp>

#if defined(ESP32)
#include <mutex>
#define GUARD_LIST_LOCK std::lock_guard<std::mutex> _guard(mutex)
#else
#define GUARD_LIST_LOCK
#endif

/**
 * @brief   Class for objects that listen for incoming MIDI Note events.
 * 
 * @ingroup MIDIInputElements
 */
class MIDIInputElementNote : public MIDIInputElement,
                             public DoublyLinkable<MIDIInputElementNote> {
  protected:
    MIDIInputElementNote() {} // not used, only for virtual inheritance

  public:
    /**
     * @brief   Constructor.
     * @todo    Documentation.
     */
    MIDIInputElementNote(const MIDICNChannelAddress &address)
        : MIDIInputElement{address} {
        GUARD_LIST_LOCK;
        elements.append(this);
    }

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
    static void updateAllWith(const MIDI_message_matcher &midimsg) {
        for (MIDIInputElementNote &e : elements)
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

    static DoublyLinkedList<MIDIInputElementNote> elements;
#ifdef ESP32
    static std::mutex mutex;
#endif
};

#undef GUARD_LIST_LOCK