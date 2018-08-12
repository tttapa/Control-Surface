#pragma once

#ifndef ARDUINO_ARCH_ESP32
#error "MIDI over Bluetooth is only supported on ESP32 boards"
#else

#include "MIDI_Interface.h"

#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

constexpr const char *SERVICE_UUID = "03b80e5a-ede8-4b33-a751-6ce34ec4c700";
constexpr const char *CHARACTERISTIC_UUID = "7772e5db-3868-4112-a1a9-f2669d106bf3";

constexpr const char *BLE_MIDI_NAME = "Control Surface (BLE)";

class EmptyParser : public MIDI_Parser {};

class BluetoothMIDI_Interface : public MIDI_Interface,
                                public BLEServerCallbacks,
                                public BLECharacteristicCallbacks {
    void onConnect(BLEServer *pServer) override { DEBUGFN("Connected"); };
    void onDisconnect(BLEServer *pServer) override { DEBUGFN("Disonnected"); }

    void onRead(BLECharacteristic *pCharacteristic) override {
        DEBUGFN("Read");
    }
    void onWrite(BLECharacteristic *pCharacteristic) override {
        DEBUGFN("Write");
        // TODO
    }

    constexpr static unsigned long MAX_MESSAGE_TIME = 10000; // microseconds

    unsigned long startTime = 0;

    constexpr static size_t BUFFER_LENGTH = 128;

    uint8_t buffer[BUFFER_LENGTH] = {};
    size_t index = 0;

    BLECharacteristic *pCharacteristic;
    BLE2902 descriptor;

    EmptyParser parser;

    bool hasSpaceFor(size_t bytes) { return index + bytes < BUFFER_LENGTH; }

  public:
    BluetoothMIDI_Interface() : MIDI_Interface(parser) {}

    void publish() {
        if (index == 0)
            return;
        pCharacteristic->setValue(buffer, index);
        pCharacteristic->notify();
        index = 0;
    }

    void begin() override {
        BLEDevice::init(BLE_MIDI_NAME);
        BLEServer *pServer = BLEDevice::createServer();
        pServer->setCallbacks(this);

        BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID));

        pCharacteristic = pService->createCharacteristic(
            BLEUUID(CHARACTERISTIC_UUID),
            BLECharacteristic::PROPERTY_READ |
                BLECharacteristic::PROPERTY_WRITE |
                BLECharacteristic::PROPERTY_NOTIFY |
                BLECharacteristic::PROPERTY_WRITE_NR);

        pCharacteristic->setCallbacks(this);
        pCharacteristic->addDescriptor(&descriptor);

        pService->start();
        pServer->getAdvertising()->start();
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
        }

        if (first) {
            startTime = micros();
        }

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
};

#endif