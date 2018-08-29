#pragma once

#include "BLEMIDI.hpp"
#include "SerialMIDI_Interface.h"

#include <Helpers/Error.hpp>

/**
 * @brief   Bluetooth Low Energy MIDI Interface for the ESP32.
 * 
 * @ingroup MIDIInterfaces
 */
class BluetoothMIDI_Interface : public MIDI_Interface,
                                public BLEServerCallbacks,
                                public BLECharacteristicCallbacks {

    // BLE Callbacks

    void onConnect(UNUSED_PARAM BLEServer *pServer) override {
        DEBUGFN("Connected");
        connected++;
    };
    void onDisconnect(UNUSED_PARAM BLEServer *pServer) override {
        DEBUGFN("Disonnected");
        if (!connected) {
            ERROR(F("Error: disconnect event, but was not connected"), 0x7788);
            return;
        }
        connected--;
    }

    void onRead(BLECharacteristic *pCharacteristic) override {
        DEBUGFN("Read");
        pCharacteristic->setValue(nullptr, 0);
    }
    void onWrite(BLECharacteristic *pCharacteristic) override {
        DEBUGFN("Write: ");
        std::string value = pCharacteristic->getValue();
        const uint8_t *const data =
            reinterpret_cast<const uint8_t *const>(value.data());
        size_t len = value.size();
        for (size_t i = 0; i < len; i++) {
            Serial.print(data[i], HEX);
            Serial.print(' ');
        }
        Serial.println();
        parse(data, len);
    }

    constexpr static unsigned long MAX_MESSAGE_TIME = 10000; // microseconds

    unsigned long startTime = 0;

    constexpr static size_t BUFFER_LENGTH = 128;

    uint8_t buffer[BUFFER_LENGTH] = {};
    size_t index = 0;

    SerialMIDI_Parser parser;

    BLEMIDI bleMidi;

    uint8_t connected = 0;

    bool hasSpaceFor(size_t bytes) { return index + bytes < BUFFER_LENGTH; }

  public:
    BluetoothMIDI_Interface() : MIDI_Interface(parser) {}

    void begin() override {
        bleMidi.begin();
        bleMidi.setServerCallbacks(this);
        bleMidi.setCharacteristicsCallbacks(this);
    }

    void publish() {
        if (index == 0)
            return;
        if (!connected) {
            DEBUGFN("No connected BLE clients");
            return;
        }
        bleMidi.notifyValue(buffer, index);
        index = 0;
    }

    MIDI_read_t read() override {
        update();          // TODO
        return NO_MESSAGE; // TODO
    }

    template <size_t N>
    void addToBuffer(uint8_t (&data)[N]) {
        addToBuffer(&data[0], N);
    }

    void addToBuffer(uint8_t *data, size_t len) {
        bool first = index == 0;
        if (!hasSpaceFor(len + 1 + first)) { // TODO
            DEBUGFN("Buffer full");
            publish();
            if (!hasSpaceFor(len + 1 + first)) { // TODO
                DEBUGFN("Message is larger than buffer");
                return;
            }
        }

        if (first)
            startTime = micros();

        if (first)
            buffer[index++] = 0x80; // header / timestamp msb
        buffer[index++] = 0x80;     // timestamp lsb
        memcpy(&buffer[index], data, len);
        index += len;

        update();
    }

    void update() {
        if (micros() - startTime >= MAX_MESSAGE_TIME)
            publish();
    }

    void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2) override {
        uint8_t msg[3] = {uint8_t(m | c), d1, d2};
        addToBuffer(msg);
    }
    void sendImpl(uint8_t m, uint8_t c, uint8_t d1) override {
        uint8_t msg[2] = {uint8_t(m | c), d1};
        addToBuffer(msg);
    }

    void parse(const uint8_t *const data, const size_t len) {
        if (len <= 1)
            return;
        if (MIDI_Parser::isData(data[0]))
            return;
        if (MIDI_Parser::isData(data[1]))
            parse(data[1]);
        bool prevWasTimestamp = true;
        for (const uint8_t *d = data + 2; d < data + len; d++) {
            if (MIDI_Parser::isData(*d)) {
                parse(*d);
                prevWasTimestamp = false;
            } else {
                if (prevWasTimestamp)
                    parse(*d);
                prevWasTimestamp = !prevWasTimestamp;
            }
        }
    }

    void parse(uint8_t data) {
        MIDI_read_t result = parser.parse(data);
        switch (result) {
            case NO_MESSAGE: break;
            case CHANNEL_MESSAGE: onChannelMessage(); break;
            case SYSEX_MESSAGE: onSysExMessage(); break;
        }
    }

    BLEMIDI &getBLEMIDI() { return bleMidi; }
};