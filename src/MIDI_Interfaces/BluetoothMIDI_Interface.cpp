#if !defined(ARDUINO) || defined(ESP32) || defined(DOXYGEN)

#include "BluetoothMIDI_Interface.hpp"
#include "BLEMIDI/ESP32/midi.h"

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

// The following section defines functions that send the MIDI BLE packets in
// the background.

void BluetoothMIDI_Interface::startSendingThread() {
    // As long as you didn't get the stop signal, wait for data to send
    auto send_loop = [this] {
        while (handleSendEvents())
            ; // loop
    };
    // Need larger stack than default, pin to non-Arduino core
    ScopedThreadConfig(4096, 3, true, "SendBLEMIDI", 0);
    // Launch the thread
    send_thread = std::thread(send_loop);
}

bool BluetoothMIDI_Interface::handleSendEvents() {
    lock_t lock(mtx);

    // Wait for a packet to be started (or for a stop signal)
    cv.wait(lock, [this] { return !packetbuilder.empty() || stop_sending; });
    bool keep_going = !stop_sending;
    // Wait for flush signal or timeout
    bool flushing = cv.wait_for(lock, timeout, [this] { return flushnow; });

    // Send the packet over BLE, empty the buffer, and update the buffer size
    // based on the MTU of the connected clients.
    if (!packetbuilder.empty())
        notifyMIDIBLE(packetbuilder.getPacket());
    packetbuilder.reset();
    packetbuilder.setCapacity(min_mtu - 3);

    // Notify the main thread that the flush was done
    if (flushing) {
        flushnow = false;
        lock.unlock();
        cv.notify_one();
    }
    return keep_going;
}

void BluetoothMIDI_Interface::flushImpl(lock_t &lock) {
    assert(lock.owns_lock());
    // No need to send empty packets
    if (packetbuilder.empty())
        return;

    // Tell the background sender thread to send the packet now
    flushnow = true;
    lock.unlock();
    cv.notify_one();

    // Wait for flush to complete (when the sender clears the flushnow flag)
    lock.lock();
    cv.wait(lock, [this] { return !flushnow; });
    assert(lock.owns_lock());
}

void BluetoothMIDI_Interface::stopSendingThread() {
    // Tell the sender that this is the last packet
    stop_sending = true;

    // Tell it to send the packet right now (flush)
    lock_t lock(mtx);
    flushnow = true;
    lock.unlock();
    cv.notify_one();

    // Wait for it to be sent, and join the thread when done
    if (send_thread.joinable())
        send_thread.join();
}

// -------------------------------------------------------------------------- //

#ifdef ARDUINO
void BluetoothMIDI_Interface::notifyMIDIBLE(
    const std::vector<uint8_t> &packet) {
    midi_notify(packet.data(), packet.size());
}
#endif

// -------------------------------------------------------------------------- //

// The following section implements the MIDI sending functions.

void BluetoothMIDI_Interface::sendChannelMessageImpl(ChannelMessage msg) {
    msg.hasTwoDataBytes() ? sendChannelMessageImpl3Bytes(msg)
                          : sendChannelMessageImpl2Bytes(msg);
}

void BluetoothMIDI_Interface::sendChannelMessageImpl3Bytes(ChannelMessage msg) {
    // BLE packets are sent asynchronously, so we need a lock to access the
    // packet buffer
    lock_t lock(mtx);
    uint16_t timestamp = millis();
    // Try adding the message to the current packet
    if (!packetbuilder.add3B(msg.header, msg.data1, msg.data2, timestamp)) {
        // If that doesn't work, flush the packet (send it now and wait until
        // it is sent)
        flushImpl(lock);
        // And then add it to the (now emtpy) buffer
        packetbuilder.add3B(msg.header, msg.data1, msg.data2, timestamp);
    }
    // Notify the packet sender that data has been added to the buffer
    lock.unlock();
    cv.notify_one();
}

void BluetoothMIDI_Interface::sendChannelMessageImpl2Bytes(ChannelMessage msg) {
    // For comments, see
    //   sendChannelMessageImpl3Bytes() above
    lock_t lock(mtx);
    uint16_t timestamp = millis();
    if (!packetbuilder.add2B(msg.header, msg.data1, timestamp)) {
        flushImpl(lock);
        packetbuilder.add2B(msg.header, msg.data1, timestamp);
    }
    lock.unlock();
    cv.notify_one();
}

void BluetoothMIDI_Interface::sendRealTimeImpl(RealTimeMessage msg) {
    // For comments, see
    //   sendChannelMessageImpl3Bytes() above
    lock_t lock(mtx);
    uint16_t timestamp = millis();
    if (!packetbuilder.addRealTime(msg.message, timestamp)) {
        flushImpl(lock);
        packetbuilder.addRealTime(msg.message, timestamp);
    }
    lock.unlock();
    cv.notify_one();
}

void BluetoothMIDI_Interface::sendSysCommonImpl(SysCommonMessage msg) {
    // For comments, see
    //   sendChannelMessageImpl3Bytes() above
    lock_t lock(mtx);
    uint16_t timestamp = millis();
    uint8_t num_data = msg.getNumberOfDataBytes();
    if (!packetbuilder.addSysCommon(num_data, msg.header, msg.data1, msg.data2,
                                    timestamp)) {
        flushImpl(lock);
        packetbuilder.addSysCommon(num_data, msg.header, msg.data1, msg.data2,
                                   timestamp);
    }
    lock.unlock();
    cv.notify_one();
}

void BluetoothMIDI_Interface::sendSysExImpl(SysExMessage msg) {
    lock_t lock(mtx);

    size_t length = msg.length;
    const uint8_t *data = msg.data;
    uint16_t timestamp = millis(); // BLE MIDI timestamp
    // TODO: I have no idea why, but the last byte gets cut off when the LSB
    // of the timestamp is 0x77 ... (Problem is probably in the BlueZ parser)
    if ((timestamp & 0x77) == 0x77)
        timestamp &= 0xFFFE;

    // Try adding at least the SysExStart header to the current packet
    if (!packetbuilder.addSysEx(data, length, timestamp)) {
        // If that didn't fit, flush the packet
        flushImpl(lock);
        // Add the first part of the SysEx message to this packet
        packetbuilder.addSysEx(data, length, timestamp);
    }
    // As long as there's data to be sent in the next packet
    while (data) {
        // Send the previous (full) packet
        flushImpl(lock);
        // And add the next part of the SysEx message to a continuation packet
        packetbuilder.continueSysEx(data, length, timestamp);
    }
    // Notify the packet sender that data has been added to the buffer
    lock.unlock();
    cv.notify_one();
}

// -------------------------------------------------------------------------- //

void BluetoothMIDI_Interface::parse(const uint8_t *const data,
                                    const size_t len) {
    auto mididata = BLEMIDIParser(data, len);
    MIDIReadEvent event = parser.pull(mididata);
    // TODO: add a timeout instead of busy waiting?
    while (event != MIDIReadEvent::NO_MESSAGE) {
        switch (event) {
            case MIDIReadEvent::CHANNEL_MESSAGE:
                while (!queue.push(parser.getChannelMessage(),
                                   mididata.getTimestamp()))
                    std::this_thread::yield();
                break;
            case MIDIReadEvent::SYSEX_CHUNK: // fallthrough
            case MIDIReadEvent::SYSEX_MESSAGE:
                while (!queue.push(parser.getSysExMessage(),
                                   mididata.getTimestamp()))
                    std::this_thread::yield();
                break;
            case MIDIReadEvent::REALTIME_MESSAGE:
                while (!queue.push(parser.getRealTimeMessage(),
                                   mididata.getTimestamp()))
                    std::this_thread::yield();
                break;
            case MIDIReadEvent::SYSCOMMON_MESSAGE:
                while (!queue.push(parser.getSysCommonMessage(),
                                   mididata.getTimestamp()))
                    std::this_thread::yield();
                break;
            case MIDIReadEvent::NO_MESSAGE: break; // LCOV_EXCL_LINE
            default: break;                        // LCOV_EXCL_LINE
        }
        event = parser.pull(mididata);
    }
    parser.cancelRunningStatus();
}

MIDIReadEvent BluetoothMIDI_Interface::read() {
    // Pop a new message from the queue
    if (!queue.pop(incomingMessage))
        return MIDIReadEvent::NO_MESSAGE;
    return incomingMessage.eventType;
}

ChannelMessage BluetoothMIDI_Interface::getChannelMessage() const {
    return incomingMessage.eventType == MIDIReadEvent::CHANNEL_MESSAGE
               ? incomingMessage.message.channelmessage
               : ChannelMessage(0, 0, 0);
}

SysCommonMessage BluetoothMIDI_Interface::getSysCommonMessage() const {
    return incomingMessage.eventType == MIDIReadEvent::SYSCOMMON_MESSAGE
               ? incomingMessage.message.syscommonmessage
               : SysCommonMessage(0, 0, 0);
}

RealTimeMessage BluetoothMIDI_Interface::getRealTimeMessage() const {
    return incomingMessage.eventType == MIDIReadEvent::REALTIME_MESSAGE
               ? incomingMessage.message.realtimemessage
               : RealTimeMessage(0);
}

SysExMessage BluetoothMIDI_Interface::getSysExMessage() const {
    auto evt = incomingMessage.eventType;
    bool hasSysEx = evt == MIDIReadEvent::SYSEX_MESSAGE ||
                    evt == MIDIReadEvent::SYSEX_CHUNK;
    return hasSysEx ? incomingMessage.message.sysexmessage
                    : SysExMessage(nullptr, 0);
}

uint16_t BluetoothMIDI_Interface::getTimestamp() const {
    return incomingMessage.timestamp;
}

// -------------------------------------------------------------------------- //

void BluetoothMIDI_Interface::updateMTU(uint16_t mtu) {
    uint16_t force_min_mtu_c = force_min_mtu;
    if (force_min_mtu_c == 0)
        min_mtu = mtu;
    else
        min_mtu = std::min(force_min_mtu_c, mtu);
    DEBUGFN(NAMEDVALUE(min_mtu));
    lock_t lock(mtx);
    if (packetbuilder.getSize() == 0)
        packetbuilder.setCapacity(min_mtu - 3);
}

void BluetoothMIDI_Interface::forceMinMTU(uint16_t mtu) {
    force_min_mtu = mtu;
    updateMTU(min_mtu);
}

// -------------------------------------------------------------------------- //

extern "C" void BluetoothMIDI_Interface_midi_mtu_callback(uint16_t mtu) {
    BluetoothMIDI_Interface::midi_mtu_callback(mtu);
}

extern "C" void BluetoothMIDI_Interface_midi_write_callback(const uint8_t *data,
                                                            size_t length) {
    BluetoothMIDI_Interface::midi_write_callback(data, length);
}

// -------------------------------------------------------------------------- //

void BluetoothMIDI_Interface::begin() {
#ifdef ARDUINO
    midi_set_mtu_callback(BluetoothMIDI_Interface_midi_mtu_callback);
    midi_set_write_callback(BluetoothMIDI_Interface_midi_write_callback);
    DEBUGFN(F("Initializing BLE MIDI Interface"));
    if (!midi_init()) {
        ERROR(F("Error initializing BLE MIDI interface"), 0x2022);
        return;
    }
#endif
    startSendingThread();
}

void BluetoothMIDI_Interface::end() {
#ifdef ARDUINO
    DEBUGFN(F("Deinitializing BLE MIDI Interface"));
    if (!midi_deinit()) {
        ERROR(F("Error deinitializing BLE MIDI interface"), 0x2023);
        return;
    }
#endif
}

// -------------------------------------------------------------------------- //

BluetoothMIDI_Interface *BluetoothMIDI_Interface::instance = nullptr;

END_CS_NAMESPACE

#endif