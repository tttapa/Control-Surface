#if !defined(ARDUINO) || defined(ESP32) || defined(DOXYGEN)

#include "GenericBLEMIDI_Interface.hpp"

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

// The following section implements the MIDI sending functions.

template <class L, class F>
void GenericBLEMIDI_Interface::sendImpl(L &lck, F add_to_buffer) {
    // BLE packets are sent asynchronously, so we need a lock to access the
    // packet buffer
    assert(lck.lck.owns_lock());
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

void GenericBLEMIDI_Interface::sendChannelMessageImpl(ChannelMessage msg) {
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

void GenericBLEMIDI_Interface::sendRealTimeImpl(RealTimeMessage msg) {
    uint16_t timestamp = millis(); // BLE MIDI timestamp
    auto lck = backend.acquirePacket();
    sendImpl(lck,
             [&] { return lck.packet->addRealTime(msg.message, timestamp); });
}

void GenericBLEMIDI_Interface::sendSysCommonImpl(SysCommonMessage msg) {
    uint16_t timestamp = millis(); // BLE MIDI timestamp
    auto lck = backend.acquirePacket();
    sendImpl(lck, [&] {
        return lck.packet->addSysCommon(msg.getNumberOfDataBytes(), msg.header,
                                        msg.data1, msg.data2, timestamp);
    });
}

void GenericBLEMIDI_Interface::sendSysExImpl(SysExMessage msg) {
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

// -------------------------------------------------------------------------- //

void GenericBLEMIDI_Interface::parseNowThreadSafe(BLEDataGenerator ble_data) {
    auto chunk = ble_data();
    if (chunk.length == 0)
        return;
    ble_parser = {chunk.data, chunk.length};
    while (true) {
        MIDIReadEvent event = parser.pull(ble_parser);
        while (event != MIDIReadEvent::NO_MESSAGE) {
            switch (event) {
                case MIDIReadEvent::CHANNEL_MESSAGE:
                    if (!queue.push(parser.getChannelMessage(),
                                    ble_parser.getTimestamp()))
                        queueFull();
                    break;
                case MIDIReadEvent::SYSEX_CHUNK: // fallthrough
                case MIDIReadEvent::SYSEX_MESSAGE:
                    if (!queue.push(parser.getSysExMessage(),
                                    ble_parser.getTimestamp()))
                        queueFull();
                    break;
                case MIDIReadEvent::REALTIME_MESSAGE:
                    if (!queue.push(parser.getRealTimeMessage(),
                                    ble_parser.getTimestamp()))
                        queueFull();
                    break;
                case MIDIReadEvent::SYSCOMMON_MESSAGE:
                    if (!queue.push(parser.getSysCommonMessage(),
                                    ble_parser.getTimestamp()))
                        queueFull();
                    break;
                case MIDIReadEvent::NO_MESSAGE: break; // LCOV_EXCL_LINE
                default: break;                        // LCOV_EXCL_LINE
            }
            event = parser.pull(ble_parser);
        }
        chunk = ble_data();
        if (chunk.length == 0)
            break;
        ble_parser.extend(chunk.data, chunk.length);
    }
    parser.cancelRunningStatus();
}

MIDIReadEvent GenericBLEMIDI_Interface::read() {
    // Pop a new message from the queue
    if (!queue.pop(incomingMessage))
        return MIDIReadEvent::NO_MESSAGE;
    return incomingMessage.eventType;
}

ChannelMessage GenericBLEMIDI_Interface::getChannelMessage() const {
    return incomingMessage.eventType == MIDIReadEvent::CHANNEL_MESSAGE
               ? incomingMessage.message.channelmessage
               : ChannelMessage(0, 0, 0);
}

SysCommonMessage GenericBLEMIDI_Interface::getSysCommonMessage() const {
    return incomingMessage.eventType == MIDIReadEvent::SYSCOMMON_MESSAGE
               ? incomingMessage.message.syscommonmessage
               : SysCommonMessage(0, 0, 0);
}

RealTimeMessage GenericBLEMIDI_Interface::getRealTimeMessage() const {
    return incomingMessage.eventType == MIDIReadEvent::REALTIME_MESSAGE
               ? incomingMessage.message.realtimemessage
               : RealTimeMessage(0);
}

SysExMessage GenericBLEMIDI_Interface::getSysExMessage() const {
    auto evt = incomingMessage.eventType;
    bool hasSysEx = evt == MIDIReadEvent::SYSEX_MESSAGE ||
                    evt == MIDIReadEvent::SYSEX_CHUNK;
    return hasSysEx ? incomingMessage.message.sysexmessage
                    : SysExMessage(nullptr, 0);
}

uint16_t GenericBLEMIDI_Interface::getTimestamp() const {
    return incomingMessage.timestamp;
}

// -------------------------------------------------------------------------- //

void GenericBLEMIDI_Interface::setName(const char *name) {
    ble_settings.device_name = name;
}

void GenericBLEMIDI_Interface::begin() { backend.begin(ble_settings); }

void GenericBLEMIDI_Interface::end() {
    // TODO
}

// -------------------------------------------------------------------------- //

END_CS_NAMESPACE

#endif