#pragma once

#ifdef ARDUINO

#ifndef ARDUINO_ARCH_ESP32
#error "MIDI over Bluetooth is only supported on ESP32 boards"
#endif

#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

// https://www.midi.org/specifications/item/bluetooth-le-midi
const char *SERVICE_UUID = "03b80e5a-ede8-4b33-a751-6ce34ec4c700";
const char *CHARACTERISTIC_UUID = "7772e5db-3868-4112-a1a9-f2669d106bf3";

constexpr const char *BLE_MIDI_NAME = "Control Surface (BLE)";

class BLEMIDI {
  public:
    ~BLEMIDI() {
        // TODO
    }

    /** 
     * @note    Must be called after begin.
     */
    void setServerCallbacks(BLEServerCallbacks *cb) {
        if (pServer == nullptr) {
            DEBUGFN("Error: pServer has not been initialized");
            ERROR(return );
        }
        pServer->setCallbacks(cb);
    }

    /** 
     * @note    Must be called after begin.
     */
    void setCharacteristicsCallbacks(BLECharacteristicCallbacks *cb) {
        if (pCharacteristic == nullptr) {
            DEBUGFN("Error: Characteristic has not been initialized");
            ERROR(return );
        }
        pCharacteristic->setCallbacks(cb);
    }

    void begin() {
        // DEBUGFN("");
        if (BLEDevice::getInitialized()) {
            DEBUGFN("Error: BLEDevice is initialized already");
            ERROR(return ); // TODO: What to do here?
        }
        BLEDevice::init(BLE_MIDI_NAME);
        pServer = BLEDevice::createServer();

        BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID));

        pCharacteristic = pService->createCharacteristic(
            BLEUUID(CHARACTERISTIC_UUID),
            BLECharacteristic::PROPERTY_READ |
                BLECharacteristic::PROPERTY_WRITE_NR |
                BLECharacteristic::PROPERTY_NOTIFY);

        descriptor.setNotifications(true);
        pCharacteristic->addDescriptor(&descriptor);

        pService->start();
        pServer->startAdvertising();
    }

    void notifyValue(uint8_t *data, size_t len) {
        pCharacteristic->setValue(data, len);
        pCharacteristic->notify();
    }

    std::string getValue() { return pCharacteristic->getValue(); }

  private:
    BLECharacteristic *pCharacteristic = nullptr;
    BLEServer *pServer = nullptr;
    BLE2902 descriptor;
};

#else

#include <gmock/gmock.h>
#include <string>

class BLECharacteristic {
  public:
    MOCK_METHOD0(getValue, std::string(void));
    MOCK_METHOD2(setValue, void(uint8_t *data, size_t len));
};

class BLECharacteristicCallbacks {
  public:
    virtual ~BLECharacteristicCallbacks() = default;
    virtual void onRead(__attribute__((unused))
                        BLECharacteristic *pCharacteristic) {}
    virtual void onWrite(__attribute__((unused))
                         BLECharacteristic *pCharacteristic) {}
};

class BLEServer {};

class BLEServerCallbacks {
  public:
    virtual ~BLEServerCallbacks() = default;
    virtual void onConnect(__attribute__((unused)) BLEServer *pServer) {}
    virtual void onDisconnect(__attribute__((unused)) BLEServer *pServer) {}
};

class BLEMIDI {
  public:
    MOCK_METHOD1(setServerCallbacks, void(BLEServerCallbacks *));
    MOCK_METHOD1(setCharacteristicsCallbacks,
                 void(BLECharacteristicCallbacks *));
    MOCK_METHOD0(begin, void(void));
    MOCK_METHOD2(notifyValue, void(uint8_t *data, size_t len));
    MOCK_METHOD0(getValue, std::string(void));
};

#endif