#include "GenericBLEMIDI_Interface.hpp"

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

// The following section implements the MIDI sending functions.

template <class BackendT>
template <class L, class F>
void GenericBLEMIDI_Interface<BackendT>::sendImpl(L &lck, F add_to_buffer) {
    // BLE packets are sent asynchronously, so we need a lock to access the
    // packet buffer
    //     assert(lck.lck.owns_lock());

    // Try adding the message to the current packet
    if (!add_to_buffer()) {
        // If that doesn't work, flush the packet (send it now and wait until
        // it is sent)
        backend.sendNow(lck);
        // And then add it to the (now emtpy) buffer
        add_to_buffer();
    }
    // Notify the sending thread that data has been added to the buffer
    backend.releasePacketAndNotify(lck);
}

template <class BackendT>
void GenericBLEMIDI_Interface<BackendT>::sendChannelMessageImpl(
    ChannelMessage msg) {
    uint16_t timestamp = millis(); // BLE MIDI timestamp
    auto lck = backend.acquirePacket();
    if (msg.hasTwoDataBytes()) {
        sendImpl(lck, [&] {
            return lck.packet->add3B(msg.header, msg.data1, msg.data2,
                                     timestamp);
        });
    } else {
        sendImpl(lck, [&] {
            return lck.packet->add2B(msg.header, msg.data1, timestamp);
        });
    }
}

template <class BackendT>
void GenericBLEMIDI_Interface<BackendT>::sendRealTimeImpl(RealTimeMessage msg) {
    uint16_t timestamp = millis(); // BLE MIDI timestamp
    auto lck = backend.acquirePacket();
    sendImpl(lck,
             [&] { return lck.packet->addRealTime(msg.message, timestamp); });
}

template <class BackendT>
void GenericBLEMIDI_Interface<BackendT>::sendSysCommonImpl(
    SysCommonMessage msg) {
    uint16_t timestamp = millis(); // BLE MIDI timestamp
    auto lck = backend.acquirePacket();
    sendImpl(lck, [&] {
        return lck.packet->addSysCommon(msg.getNumberOfDataBytes(), msg.header,
                                        msg.data1, msg.data2, timestamp);
    });
}

template <class BackendT>
void GenericBLEMIDI_Interface<BackendT>::sendSysExImpl(SysExMessage msg) {
    uint16_t timestamp = millis(); // BLE MIDI timestamp
    size_t length = msg.length;
    const uint8_t *data = msg.data;

    // BLE packets are sent asynchronously, so we need a lock to access the
    // packet buffer
    auto lck = backend.acquirePacket();

    // TODO: I have no idea why, but the last byte gets cut off when the LSB
    // of the timestamp is 0x77 ... (Problem is probably in the BlueZ parser)
    if ((timestamp & 0x77) == 0x77)
        timestamp &= 0xFFFE;

    // Try adding at least the SysExStart header to the current packet
    if (!lck.packet->addSysEx(data, length, timestamp)) {
        // If that didn't fit, flush the packet
        backend.sendNow(lck);
        // Add the first part of the SysEx message to this packet
        lck.packet->addSysEx(data, length, timestamp);
    }
    // As long as there's data to be sent in the next packet
    while (data) {
        // Send the previous (full) packet
        backend.sendNow(lck);
        // And add the next part of the SysEx message to a continuation packet
        lck.packet->continueSysEx(data, length, timestamp);
    }
    // Notify the sending thread that data has been added to the buffer
    backend.releasePacketAndNotify(lck);
}

template <class BackendT>
void GenericBLEMIDI_Interface<BackendT>::sendNowImpl() {
    auto lck = backend.acquirePacket();
    backend.sendNow(lck);
}

// -------------------------------------------------------------------------- //

template <class BackendT>
MIDIReadEvent GenericBLEMIDI_Interface<BackendT>::read() {
    // Pop a new message from the queue
    if (!backend.popMessage(incomingMessage))
        return MIDIReadEvent::NO_MESSAGE;
    return incomingMessage.eventType;
}

template <class BackendT>
ChannelMessage GenericBLEMIDI_Interface<BackendT>::getChannelMessage() const {
    return incomingMessage.eventType == MIDIReadEvent::CHANNEL_MESSAGE
               ? incomingMessage.message.channelmessage
               : ChannelMessage(0, 0, 0);
}

template <class BackendT>
SysCommonMessage
GenericBLEMIDI_Interface<BackendT>::getSysCommonMessage() const {
    return incomingMessage.eventType == MIDIReadEvent::SYSCOMMON_MESSAGE
               ? incomingMessage.message.syscommonmessage
               : SysCommonMessage(0, 0, 0);
}

template <class BackendT>
RealTimeMessage GenericBLEMIDI_Interface<BackendT>::getRealTimeMessage() const {
    return incomingMessage.eventType == MIDIReadEvent::REALTIME_MESSAGE
               ? incomingMessage.message.realtimemessage
               : RealTimeMessage(0);
}

template <class BackendT>
SysExMessage GenericBLEMIDI_Interface<BackendT>::getSysExMessage() const {
    auto evt = incomingMessage.eventType;
    bool hasSysEx = evt == MIDIReadEvent::SYSEX_MESSAGE ||
                    evt == MIDIReadEvent::SYSEX_CHUNK;
    return hasSysEx ? incomingMessage.message.sysexmessage
                    : SysExMessage(nullptr, 0);
}

template <class BackendT>
uint16_t GenericBLEMIDI_Interface<BackendT>::getTimestamp() const {
    return incomingMessage.timestamp;
}

// -------------------------------------------------------------------------- //

template <class BackendT>
void GenericBLEMIDI_Interface<BackendT>::setName(const char *name) {
    ble_settings.device_name = name;
}

template <class BackendT>
void GenericBLEMIDI_Interface<BackendT>::begin() {
    backend.begin(ble_settings);
}

template <class BackendT>
void GenericBLEMIDI_Interface<BackendT>::end() {
    backend.end();
}

// -------------------------------------------------------------------------- //

END_CS_NAMESPACE
