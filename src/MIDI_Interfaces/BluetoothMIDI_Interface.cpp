#if !defined(ARDUINO) || defined(ARDUINO_ARCH_ESP32)

#include "BluetoothMIDI_Interface.hpp"

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

// The following section defines functions that send the MIDI BLE packets in
// the background.

void BluetoothMIDI_Interface::startSendingThread() {
    // As long as you didn't get the stop signal, wait for data to send
    auto send_loop = [this] {
        while (!stop_sending)
            handleSendEvents();
    };
    // Need larger stack than default
    ScopedThreadConfig(4096, 3, true, "SendBLEMIDI");
    // Launch the thread
    send_thread = std::thread(send_loop);
}

void BluetoothMIDI_Interface::handleSendEvents() {
    lock_t lock(mtx);

    // Wait for a packet to be started (or for a stop signal)
    cv.wait(lock, [this] { return !packetbuilder.empty() || stop_sending; });
    // Wait for flush signal or timeout
    bool flushing = cv.wait_for(lock, timeout, [this] { return flushnow; });

    // Send the packet over BLE, empty the buffer, and update the buffer size
    // based on the MTU of the connected clients.
    bleMidi.notifyValue(packetbuilder.getBuffer(), packetbuilder.getSize());
    packetbuilder.reset();
    packetbuilder.setCapacity(min_mtu - 3);

    // Notify the main thread that the flush was done
    if (flushing) {
        flushnow = false;
        lock.unlock();
        cv.notify_one();
    }
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

// The following section implements the MIDI sending functions.

void BluetoothMIDI_Interface::sendImpl(uint8_t header, uint8_t d1, uint8_t d2,
                                       uint8_t) {
    // BLE packets are sent asynchronously, so we need a lock to access the
    // packet buffer
    lock_t lock(mtx);
    uint16_t timestamp = millis();
    // Try adding the message to the current packet
    if (!packetbuilder.add3B(header, d1, d2, timestamp)) {
        // If that doesn't work, flush the packet (send it now and wait until
        // it is sent)
        flushImpl(lock);
        // And then add it to the (now emtpy) buffer
        packetbuilder.add3B(header, d1, d2, timestamp);
    }
    // Notify the packet sender that data has been added to the buffer
    lock.unlock();
    cv.notify_one();
}

void BluetoothMIDI_Interface::sendImpl(uint8_t header, uint8_t d1, uint8_t) {
    // For comments, see
    //   sendImpl(uint8_t header, uint8_t d1, uint8_t d2, uint8_t) above
    lock_t lock(mtx);
    uint16_t timestamp = millis();
    if (!packetbuilder.add2B(header, d1, timestamp)) {
        flushImpl(lock);
        packetbuilder.add2B(header, d1, timestamp);
    }
    lock.unlock();
    cv.notify_one();
}

void BluetoothMIDI_Interface::sendImpl(uint8_t rt, uint8_t) {
    // For comments, see
    //   sendImpl(uint8_t header, uint8_t d1, uint8_t d2, uint8_t) above
    lock_t lock(mtx);
    uint16_t timestamp = millis();
    if (!packetbuilder.addRealTime(rt, timestamp)) {
        flushImpl(lock);
        packetbuilder.addRealTime(rt, timestamp);
    }
    lock.unlock();
    cv.notify_one();
}

void BluetoothMIDI_Interface::sendImpl(const uint8_t *data, size_t length,
                                       uint8_t) {
    // SysEx is always at least a SysExStart plus a SysExEnd, so >= 2 bytes
    if (length < 2)
        return;

    lock_t lock(mtx);

    length -= 2; // Length of SysEx data without SysEx start/end
    data += 1;   // Data without SysEx start
    uint16_t timestamp = millis(); // BLE MIDI timestamp

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

// The following section defines callback functions from the Bluetooth LE
// connection manager.

void BluetoothMIDI_Interface::onConnect(BLEServer *pServer) {
    // When a new client connects to our server, check all MTUs to know if
    // we have to shrink our packet size.-
    (void)pServer;
    DEBUGFN(F("Connected"));
    updateMTU();
}

void BluetoothMIDI_Interface::onDisconnect(BLEServer *pServer) {
    // When a client disconnects, check all MTUs to see if we can grow our
    // packet size.
    (void)pServer;
    DEBUGFN(F("Disonnected"));
    updateMTU();
}

void BluetoothMIDI_Interface::onRead(BLECharacteristic *pCharacteristic) {
    // According to the BLE-MIDI standard, we should respond to read requests
    // with no payload.
    DEBUGFN(F("Read"));
    pCharacteristic->setValue(nullptr, 0);
}

void BluetoothMIDI_Interface::onWrite(BLECharacteristic *pCharacteristic) {
    // Clients write to our server to send us MIDI data, just parse the packet.
    // Note: this is called from a Bluetooth task, which runs asynchronously,
    // independently from the main code (maybe even in parallel).
    std::string value = pCharacteristic->getValue();
    const uint8_t *data = reinterpret_cast<const uint8_t *>(value.data());
    size_t len = value.size();
    DEBUGFN(F("Write [") << len << "] " << AH::HexDump(data, len));
    parse(data, len);
}

// -------------------------------------------------------------------------- //

void BluetoothMIDI_Interface::updateMTU() {
    uint16_t force_min_mtu_c = force_min_mtu;
    uint16_t ble_min_mtu = bleMidi.getMinMTU();
    if (force_min_mtu_c == 0)
        min_mtu = ble_min_mtu;
    else
        min_mtu = std::min(force_min_mtu_c, ble_min_mtu);
    DEBUGFN(NAMEDVALUE(min_mtu));
}

void BluetoothMIDI_Interface::forceMinMTU(uint16_t mtu) {
    lock_t lock(mtx);
    force_min_mtu = mtu;
    updateMTU();
    if (packetbuilder.getSize() == 0)
        packetbuilder.setCapacity(min_mtu - 3);
}

END_CS_NAMESPACE

#endif