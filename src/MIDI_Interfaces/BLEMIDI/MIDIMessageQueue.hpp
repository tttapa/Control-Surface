#pragma once

#include <MIDI_Parsers/MIDIReadEvent.hpp>
#include <MIDI_Parsers/MIDI_MessageTypes.hpp>

#include <atomic>

BEGIN_CS_NAMESPACE

class MIDIMessageQueue {
  public:
    MIDIMessageQueue(size_t capacity) : storage(storage_t(capacity)) {}

    struct MIDIMessageQueueElement {
        MIDIReadEvent eventType = MIDIReadEvent::NO_MESSAGE;
        union Message {
            ChannelMessage channelmessage;
            SysCommonMessage syscommonmessage;
            RealTimeMessage realtimemessage;
            SysExMessage sysexmessage;

            Message() : realtimemessage(0x00) {}
            Message(ChannelMessage msg) : channelmessage(msg) {}
            Message(SysCommonMessage msg) : syscommonmessage(msg) {}
            Message(RealTimeMessage msg) : realtimemessage(msg) {}
            Message(SysExMessage msg) : sysexmessage(msg) {}
        } message;
        uint16_t timestamp = 0xFFFF;

        MIDIMessageQueueElement() = default;
        MIDIMessageQueueElement(ChannelMessage message, uint16_t timestamp)
            : eventType(MIDIReadEvent::CHANNEL_MESSAGE), message(message),
              timestamp(timestamp) {}
        MIDIMessageQueueElement(SysCommonMessage message, uint16_t timestamp)
            : eventType(MIDIReadEvent::SYSCOMMON_MESSAGE), message(message),
              timestamp(timestamp) {}
        MIDIMessageQueueElement(RealTimeMessage message, uint16_t timestamp)
            : eventType(MIDIReadEvent::REALTIME_MESSAGE), message(message),
              timestamp(timestamp) {}
        MIDIMessageQueueElement(SysExMessage message, uint16_t timestamp);

        /// No copy constructor.
        MIDIMessageQueueElement(const MIDIMessageQueueElement &) = delete;
        /// No copy assignment.
        MIDIMessageQueueElement &
        operator=(const MIDIMessageQueueElement &) = delete;
        /// Move constructor.
        MIDIMessageQueueElement(MIDIMessageQueueElement &&that);
        /// Move assignemnt.
        MIDIMessageQueueElement &operator=(MIDIMessageQueueElement &&that);

        /// Deallocate the storage for the SysEx data (if present).
        void release();

        ~MIDIMessageQueueElement() { release(); }
    };

    using storage_t = std::vector<MIDIMessageQueueElement>;
    using iter_t = storage_t::iterator;

  public:
    bool push(ChannelMessage message, uint16_t timestamp);
    bool push(SysCommonMessage message, uint16_t timestamp);
    bool push(RealTimeMessage message, uint16_t timestamp);
    bool push(SysExMessage message, uint16_t timestamp);

    bool pop(MIDIMessageQueueElement &message);

  private:
    storage_t storage = storage_t(64);
    iter_t write_p = storage.begin();
    iter_t read_p = storage.begin();
    std::atomic_size_t size{0};

    bool push(MIDIMessageQueueElement &&message);

    void inc(iter_t &it) {
        if (++it == storage.end())
            it = storage.begin();
    }
};

END_CS_NAMESPACE
