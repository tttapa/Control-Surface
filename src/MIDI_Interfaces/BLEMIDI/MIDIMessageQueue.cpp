#if defined(ESP32) || !defined(ARDUINO)

#include "MIDIMessageQueue.hpp"

BEGIN_CS_NAMESPACE

MIDIMessageQueue::MIDIMessageQueueElement::MIDIMessageQueueElement(
    SysExMessage message, uint16_t timestamp)
    : eventType(message.isLastChunk() ? MIDIReadEvent::SYSEX_MESSAGE
                                      : MIDIReadEvent::SYSEX_CHUNK),
      message(message), timestamp(timestamp) {
    // SysEx data is copied, not stored by pointer, so allocate new
    // storage for the SysEx data, and copy it if allocation was
    // successful.
    uint8_t *newBuffer = new uint8_t[message.length];
    // Make sure that allocation was successful.
    if (newBuffer == nullptr) {
        DEBUGREF(F("SysEx buffer allocation failed"));
        this->eventType = MIDIReadEvent::NO_MESSAGE;
        this->message.realtimemessage = 0x00;
    } else {
        // Copy the SysEx data to the newly allocated buffer
        memcpy(newBuffer, message.data, message.length);
        this->message.sysexmessage.data = newBuffer;
    }
}

MIDIMessageQueue::MIDIMessageQueueElement::MIDIMessageQueueElement(
    MIDIMessageQueue::MIDIMessageQueueElement &&that) {
    *this = std::move(that);
}

MIDIMessageQueue::MIDIMessageQueueElement &
MIDIMessageQueue::MIDIMessageQueueElement::operator=(
    MIDIMessageQueue::MIDIMessageQueueElement &&that) {
    std::swap(this->eventType, that.eventType);
    std::swap(this->message, that.message);
    std::swap(this->timestamp, that.timestamp);
    return *this;
}

void MIDIMessageQueue::MIDIMessageQueueElement::release() {
    if (eventType == MIDIReadEvent::SYSEX_CHUNK ||
        eventType == MIDIReadEvent::SYSEX_MESSAGE) {
        delete[] message.sysexmessage.data;
        message.sysexmessage.data = nullptr;
        message.sysexmessage.length = 0;
        eventType = MIDIReadEvent::NO_MESSAGE;
        message.realtimemessage = 0x00;
    }
}

bool MIDIMessageQueue::push(ChannelMessage message, uint16_t timestamp) {
    return push(MIDIMessageQueueElement(message, timestamp));
}

bool MIDIMessageQueue::push(SysCommonMessage message, uint16_t timestamp) {
    return push(MIDIMessageQueueElement(message, timestamp));
}

bool MIDIMessageQueue::push(RealTimeMessage message, uint16_t timestamp) {
    return push(MIDIMessageQueueElement(message, timestamp));
}

bool MIDIMessageQueue::push(SysExMessage message, uint16_t timestamp) {
    if (storage.size() == size.load(std::memory_order_acquire))
        return false;

    // Allocate storage for the actual SysEx data and copy the data
    MIDIMessageQueueElement el(message, timestamp);
    // Check if allocation failed
    if (el.eventType == MIDIReadEvent::NO_MESSAGE)
        return true; // TODO: should we try again later?

    *write_p = std::move(el);
    inc(write_p);
    size.fetch_add(1, std::memory_order_release);

    return true;
}

bool MIDIMessageQueue::pop(MIDIMessageQueueElement &message) {
    if (size.load(std::memory_order_acquire) == 0)
        return false;
    message = std::move(*read_p);
    read_p->release(); // release memory of old message
    inc(read_p);
    size.fetch_sub(1, std::memory_order_release);
    return true;
}

bool MIDIMessageQueue::push(MIDIMessageQueueElement &&message) {
    if (storage.size() == size.load(std::memory_order_acquire))
        return false;
    *write_p = std::move(message);
    inc(write_p);
    size.fetch_add(1, std::memory_order_release);
    return true;
}

END_CS_NAMESPACE

#endif
