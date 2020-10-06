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

/// https://www.midi.org/specifications/item/bluetooth-le-midi (MMA/AMEI RP-052)
constexpr static const char *BLE_MIDI_SERVICE_UUID
    = "03b80e5a-ede8-4b33-a751-6ce34ec4c700";
/// https://www.midi.org/specifications/item/bluetooth-le-midi (MMA/AMEI RP-052)
constexpr static const char *BLE_MIDI_CHARACTERISTIC_UUID
    = "7772e5db-3868-4112-a1a9-f2669d106bf3";

/// Name of Bluetooth LE MIDI device.
constexpr static const char *BLE_MIDI_NAME = "Control Surface (BLE)";

/// Handles the Bluetooth Low Energy BLE server, characteristics, callbacks 
/// etc., but not the actual MIDI stuff.  
/// https://www.midi.org/specifications/item/bluetooth-le-midi (MMA/AMEI RP-052)
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
            BLEDevice::deinit();
        }

        // Free some of the old servers, services and characteristics
        cleanup();

        // Initialize the BLE device
        BLEDevice::init(BLE_MIDI_NAME);

        // Create the BLE server
        pServer = BLEDevice::createServer();
        setServerCallbacks(serverCallbacks);

        // Create the BLE MIDI service
        pService = pServer->createService(BLEUUID(BLE_MIDI_SERVICE_UUID));

        // Create the BLE MIDI characteristic that actually sends/receives the
        // MIDI data.
        // Spec mandates read support (respond with no payload), notify support,
        // and write support (without response).
        auto options = BLECharacteristic::PROPERTY_READ |
                       BLECharacteristic::PROPERTY_NOTIFY |
                       BLECharacteristic::PROPERTY_WRITE_NR;
        pCharacteristic = pService->createCharacteristic(
            BLEUUID(BLE_MIDI_CHARACTERISTIC_UUID),
            options);
        setCharacteristicsCallbacks(midiCallbacks);

        // Create a BLE descriptor to allow the client to disable notifications
        // org.bluetooth.descriptor.gatt.client_characteristic_configuration
        pDescriptor = new BLE2902(); // TODO: does this leak?
        pCharacteristic->addDescriptor(pDescriptor);
        // By default, we need notifications to send MIDI data
        pDescriptor->setNotifications(true);

        // Start the service
        pService->start();

        // Start advertising our presence
        BLEAdvertising *pAdvertising = pServer->getAdvertising();
        // pAdvertising is a static member of BLEDevice, so shouldn't leak
        pAdvertising->addServiceUUID(pService->getUUID());
        pAdvertising->start();
    }

    /**
     * @brief   Send the given BLE MIDI packet.
     * 
     * @param   data
     *          Pointer to the data.
     * @param   len
     *          Length of the data in bytes. Maximum length is the MTU minus 
     *          three bytes.
     */
    void notifyValue(const uint8_t *data, size_t len) {
        if (pCharacteristic == nullptr) {
            ERROR(F("Error: Characteristic has not been initialized"), 0x2023);
            return;
        }
        // TODO: file pull request to add const (espressif/arduino-esp32)
        pCharacteristic->setValue(const_cast<uint8_t *>(data), len);
        pCharacteristic->notify();
    }

    /**
     * @brief Get the received BLE MIDI packet.
     */
    std::string getValue() const {
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

  private:
    void cleanup() {
        // BLEServer leaks
        if (pServer && pService)
            pServer->removeService(pService);
        delete pService;
        delete pCharacteristic;
        
        // BLEDevice::createServer() leaks
        delete pServer;
    }
};

END_CS_NAMESPACE

#else

#include "BLEMIDI-Mock.ipp"

#endif