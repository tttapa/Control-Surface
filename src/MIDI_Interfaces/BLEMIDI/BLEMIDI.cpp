#ifdef ARDUINO_ARCH_ESP32

#include "BLEMIDI.hpp"
#include <AH/STL/algorithm>

BEGIN_CS_NAMESPACE

#if 0
void BLEMIDI::notifyValue(const uint8_t *data, size_t len) {
    if (pCharacteristic == nullptr) {
        ERROR(F("Error: Characteristic has not been initialized"), 0x2023);
        return;
    }
    if (pServer == nullptr) {
        ERROR(F("Error: Server has not been initialized"), 0x2023);
        return;
    }

    DEBUGFN("Notify [" << length << "] " << AH::HexDump(data, len));

    if (pServer->getConnectedCount() == 0) {
        DEBUGFN("Notify: No connected clients");
        return;
    }

    // Test to see if we have a 0x2902 descriptor.
    // If we do, then check to see if notification is enabled and, if not, 
    // prevent the notification.
    if (pDescriptor != nullptr && !pDescriptor->getNotifications()) {
        DEBUGFN("Notify: Notifications disabled, ignoring");
        return;
    }
    
    for (auto &peer : pServer->getPeerDevices(false)) {
        uint16_t mtu = peer.second.mtu;
        if (len > mtu - 3) {
            DEBUGFN("Message longer than MTU, dropped");
            continue;
        }
        
        esp_err_t errRc = ::esp_ble_gatts_send_indicate(
                pServer->getGattsIf(), // private :(
                peer.first,
                pCharacteristic->getHandle(), 
                len, data, 
                false); // The need_confirm = false makes this a notify.
        if (errRc != ESP_OK) {
            DEBUGFN("Error: esp_ble_gatts_send_indicate (" << errRc << ") "
                        << GeneralUtils::errorToString(errRc));
            return;
        }
    }
}
#endif

void BLEMIDI::setServerCallbacks(BLEServerCallbacks *cb) {
    if (pServer == nullptr) {
        ERROR(F("Error: Server has not been initialized"), 0x2020);
        return;
    }
    pServer->setCallbacks(cb);
}

void BLEMIDI::setCharacteristicsCallbacks(BLECharacteristicCallbacks *cb) {
    if (pCharacteristic == nullptr) {
        ERROR(F("Error: Characteristic has not been initialized"), 0x2021);
        return;
    }
    pCharacteristic->setCallbacks(cb);
}

void BLEMIDI::begin(BLEServerCallbacks *serverCallbacks,
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

void BLEMIDI::notifyValue(const uint8_t *data, size_t len) {
    if (pCharacteristic == nullptr) {
        ERROR(F("Error: Characteristic has not been initialized"), 0x2023);
        return;
    }
    // TODO: file pull request to add const (espressif/arduino-esp32)
    DEBUGFN("Notify [" << len << "] " << AH::HexDump(data, len));
    pCharacteristic->setValue(const_cast<uint8_t *>(data), len);
    pCharacteristic->notify();
}


std::string BLEMIDI::getValue() const {
    if (pCharacteristic == nullptr) {
        ERROR(F("Error: Characteristic has not been initialized"), 0x2024);
        return "";
    }
    return pCharacteristic->getValue(); 
}

void BLEMIDI::cleanup() {
    // BLEServer leaks
    if (pServer && pService)
        pServer->removeService(pService);
    delete pService;
    delete pCharacteristic;
    
    // BLEDevice::createServer() leaks
    delete pServer;
}

uint16_t BLEMIDI::get_min_mtu() const {
    if (pServer == nullptr) {
        ERROR(F("Error: Server has not been initialized"), 0x2320);
        return 23; // Set to maximum and hope for the best
    }
    auto peers = pServer->getPeerDevices(false);
    using value_t = decltype(peers)::value_type;
    auto compare = [](const value_t &a, const value_t &b) {
        return a.second.mtu < b.second.mtu;
    };
    auto min = std::min_element(std::begin(peers), std::end(peers), compare);
    if (min != std::end(peers))
        return min->second.mtu;
    return 23;
}

END_CS_NAMESPACE

#endif
