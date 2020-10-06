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
constexpr static const char *BLE_MIDI_SERVICE_UUID
    = "03b80e5a-ede8-4b33-a751-6ce34ec4c700";
constexpr static const char *BLE_MIDI_CHARACTERISTIC_UUID
    = "7772e5db-3868-4112-a1a9-f2669d106bf3";

constexpr static const char *BLE_MIDI_NAME = "Control Surface (BLE)";

/// Handles the Bluetooth Low Energy BLE server, characteristics, callbacks 
/// etc., but not the actual MIDI stuff. 
class BLEMIDI {
  public:
    ~BLEMIDI() {
        // TODO
    }

    /// @note   Must be called after begin.
    void setServerCallbacks(BLEServerCallbacks *cb) {
        if (pServer == nullptr) {
            ERROR(F("Error: Server has not been initialized"), 0x2020);
            return;
        }
        pServer->setCallbacks(cb);
    }

    /// @note   Must be called after begin.
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
            ERROR(F("Error: BLEDevice was initialized already"), 0x2022);
            BLEDevice::init(BLE_MIDI_NAME);
        }

        // Initialize the BLE device
        BLEDevice::init(BLE_MIDI_NAME);

        // BLEServer leaks
        if (pServer && pService)
            pServer->removeService(pService);
        delete pService;
        delete pCharacteristic;
        
        // BLEDevice::createServer() leaks
        delete pServer;
        
        // Create the BLE server
        pServer = BLEDevice::createServer();
        setServerCallbacks(serverCallbacks);

        // Create the BLE service
        pService = pServer->createService(BLEUUID(BLE_MIDI_SERVICE_UUID));

        // Create a BLE characteristic
        pCharacteristic = pService->createCharacteristic(
            BLEUUID(BLE_MIDI_CHARACTERISTIC_UUID),
            BLECharacteristic::PROPERTY_READ |
                BLECharacteristic::PROPERTY_NOTIFY |
                BLECharacteristic::PROPERTY_WRITE_NR);

        // Create a BLE descriptor
        pDescriptor = new BLE2902(); // TODO: does this leak?
        pCharacteristic->addDescriptor(pDescriptor);
        pDescriptor->setNotifications(true);
        setCharacteristicsCallbacks(midiCallbacks);

        // Start the service
        pService->start();

        // Start advertising
        BLEAdvertising *pAdvertising = pServer->getAdvertising();
        // pAdvertising is a static member of BLEDevice, so shouldn't leak
        pAdvertising->addServiceUUID(pService->getUUID());
        pAdvertising->start();
    }

    void notifyValue(uint8_t *data, size_t len) {
        if (pCharacteristic == nullptr) {
            ERROR(F("Error: Characteristic has not been initialized"), 0x2023);
            return;
        }
        pCharacteristic->setValue(data, len);
        pCharacteristic->notify();
    }

    std::string getValue() { 
        if (pCharacteristic == nullptr) {
            ERROR(F("Error: Characteristic has not been initialized"), 0x2024);
            return "";
        }
        return pCharacteristic->getValue(); 
    }

  private:
    BLECharacteristic *pCharacteristic = nullptr;
    BLEService *pService = nullptr;
    BLEServer *pServer = nullptr;
    BLE2902 *pDescriptor = nullptr;
};

END_CS_NAMESPACE

#else

#include "BLEMIDI-Mock.ipp"

#endif