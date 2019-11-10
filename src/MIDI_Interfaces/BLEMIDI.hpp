#pragma once

#ifdef ARDUINO

#ifndef ARDUINO_ARCH_ESP32
#error "MIDI over Bluetooth is only supported on ESP32 boards"
#endif

#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include <AH/Error/Error.hpp>

BEGIN_CS_NAMESPACE

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
            ERROR(F("Error: pServer has not been initialized"), 0x2020);
            return;
        }
        pServer->setCallbacks(cb);
    }

    /** 
     * @note    Must be called after begin.
     */
    void setCharacteristicsCallbacks(BLECharacteristicCallbacks *cb) {
        if (pCharacteristic == nullptr) {
            ERROR(F("Error: Characteristic has not been initialized"), 0x2021);
            return;
        }
        pCharacteristic->setCallbacks(cb);
    }

    void begin(BLEServerCallbacks *serverCallbacks,
               BLECharacteristicCallbacks *midiCallbacks) {
        DEBUGFN("Initializing BLE MIDI Interface");
        if (BLEDevice::getInitialized()) {
            ERROR(F("Error: BLEDevice is initialized already"), 0x2022);
            return; // TODO: What to do here?
        }

        // Initialize the BLE device
        BLEDevice::init(BLE_MIDI_NAME);

        // Create the BLE server
        pServer = BLEDevice::createServer();
        setServerCallbacks(serverCallbacks);

        // Create the BLE service
        BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID));

        // Create a BLE characteristic
        pCharacteristic = pService->createCharacteristic(
            BLEUUID(CHARACTERISTIC_UUID),
            BLECharacteristic::PROPERTY_READ |
                BLECharacteristic::PROPERTY_NOTIFY |
                BLECharacteristic::PROPERTY_WRITE_NR);

        // Create a BLE descriptor
        descriptor = new BLE2902();
        pCharacteristic->addDescriptor(descriptor);
        // descriptor.setNotifications(true);
        setCharacteristicsCallbacks(midiCallbacks);

        // Start the service
        pService->start();

        // Start advertising
        BLEAdvertising *pAdvertising = pServer->getAdvertising();
        pAdvertising->addServiceUUID(pService->getUUID());
        pAdvertising->start();
    }

    void notifyValue(uint8_t *data, size_t len) {
        pCharacteristic->setValue(data, len);
        pCharacteristic->notify();
    }

    std::string getValue() { return pCharacteristic->getValue(); }

  private:
    BLECharacteristic *pCharacteristic = nullptr;
    BLEServer *pServer = nullptr;
    BLE2902 *descriptor;
};

END_CS_NAMESPACE

#else

#include <Def/Def.hpp>
#include <gmock-wrapper.h>
#include <string>

class BLECharacteristic {
  public:
    MOCK_METHOD0(getValue, std::string(void));
    MOCK_METHOD2(setValue, void(uint8_t *data, size_t len));
};

class BLECharacteristicCallbacks {
  public:
    virtual ~BLECharacteristicCallbacks() = default;
    virtual void onRead(BLECharacteristic *pCharacteristic) {
        (void)pCharacteristic;
    }
    virtual void onWrite(BLECharacteristic *pCharacteristic) {
        (void)pCharacteristic;
    }
};

class BLEServer {};

class BLEServerCallbacks {
  public:
    virtual ~BLEServerCallbacks() = default;
    virtual void onConnect(BLEServer *pServer) { (void)pServer; }
    virtual void onDisconnect(BLEServer *pServer) { (void)pServer; }
};

BEGIN_CS_NAMESPACE

class BLEMIDI {
  public:
    MOCK_METHOD1(setServerCallbacks, void(BLEServerCallbacks *));
    MOCK_METHOD1(setCharacteristicsCallbacks,
                 void(BLECharacteristicCallbacks *));
    MOCK_METHOD2(begin,
                 void(BLEServerCallbacks *, BLECharacteristicCallbacks *));
    MOCK_METHOD2(notifyValue, void(uint8_t *data, size_t len));
    MOCK_METHOD0(getValue, std::string(void));
};

END_CS_NAMESPACE

#endif