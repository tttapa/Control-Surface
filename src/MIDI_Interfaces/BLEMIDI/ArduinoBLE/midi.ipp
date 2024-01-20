#include <ArduinoBLE.h>

#include <AH/Error/Error.hpp>
#include <Settings/NamespaceSettings.hpp>

#include <MIDI_Interfaces/BLEMIDI/BLEAPI.hpp>

BEGIN_CS_NAMESPACE

namespace arduino_ble_midi {

namespace {

BLEService midi_service {
    "03B80E5A-EDE8-4B33-A751-6CE34EC4C700",
};
BLECharacteristic midi_char {
    "7772E5DB-3868-4112-A1A9-F2669D106BF3",
    BLEWriteWithoutResponse | BLERead | BLENotify,
    512,
    false,
};
MIDIBLEInstance *midi_instance = nullptr;

bool is_midi_char(const BLECharacteristic &characteristic) {
    return strcasecmp(midi_char.uuid(), characteristic.uuid()) == 0;
}

// Here I assume that all callbacks and handlers execute in the same task/thread
// as the main program.

void on_connect([[maybe_unused]] BLEDevice central) {
    DEBUGREF("CS-BLEMIDI connected, central: " << central.address());
    if (midi_instance) {
        midi_instance->handleConnect(BLEConnectionHandle {0});
        midi_instance->handleSubscribe(BLEConnectionHandle {0},
                                       BLECharacteristicHandle {0}, true);
    }
}

void on_disconnect([[maybe_unused]] BLEDevice central) {
    DEBUGREF("CS-BLEMIDI disconnected, central: " << central.address());
    if (midi_instance)
        midi_instance->handleDisconnect(BLEConnectionHandle {});
}

void on_write([[maybe_unused]] BLEDevice central,
              BLECharacteristic characteristic) {
    DEBUGREF(
        "CS-BLEMIDI write, central: "
        << central.address() << ", char: " << characteristic.uuid()
        << ", data: [" << characteristic.valueLength() << "] "
        << AH::HexDump(characteristic.value(), characteristic.valueLength()));
    if (!is_midi_char(characteristic))
        return;
    if (!midi_instance)
        return;
    BLEDataView data {characteristic.value(),
                      static_cast<uint16_t>(characteristic.valueLength())};
    auto data_gen = [data {data}]() mutable { return std::exchange(data, {}); };
    midi_instance->handleData(
        BLEConnectionHandle {0},
        BLEDataGenerator {compat::in_place, std::move(data_gen)},
        BLEDataLifetime::ConsumeImmediately);
}

void on_read([[maybe_unused]] BLEDevice central,
             BLECharacteristic characteristic) {
    DEBUGREF("CS-BLEMIDI read, central: " << central.address() << ", char: "
                                          << characteristic.uuid());
    if (!is_midi_char(characteristic))
        return;
    characteristic.setValue(nullptr, 0);
}

} // namespace

inline bool init(MIDIBLEInstance &instance, BLESettings ble_settings) {
    midi_instance = &instance;
    // Initialize the BLE hardware
    if (!BLE.begin()) {
        ERROR(F("Starting Bluetooth® Low Energy module failed!"), 0x7532);
        return false;
    }

    // Set the local name and advertise the MIDI service
    BLE.setLocalName(ble_settings.device_name);
    BLE.setAdvertisedService(midi_service);
    // Note: advertising connection interval range not supported by ArduinoBLE

    // Configure the MIDI service and characteristic
    midi_service.addCharacteristic(midi_char);
    BLE.addService(midi_service);

    // Assign event handlers
    BLE.setEventHandler(BLEConnected, on_connect);
    BLE.setEventHandler(BLEDisconnected, on_disconnect);
    midi_char.setEventHandler(BLEWritten, on_write);
    midi_char.setEventHandler(BLERead, on_read);

    // Start advertising
    BLE.advertise();

    return true;
}

inline void poll() {
    // poll for Bluetooth® Low Energy events
    BLE.poll();
}

// TODO: there is currently no way in ArduinoBLE to request the MTU. So we
// assume the tiny default of 23 bytes.

inline void notify(BLEDataView data) {
    midi_char.writeValue(data.data, data.length);
}

} // namespace arduino_ble_midi

END_CS_NAMESPACE
