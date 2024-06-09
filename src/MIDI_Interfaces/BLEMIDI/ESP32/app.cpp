#ifdef ESP32
#include <sdkconfig.h>
#if CONFIG_BT_BLE_ENABLED

#include "advertising.h"
#include "app.h"
#include "midi.h"

namespace cs::midi_ble_bluedroid {
namespace {
MIDIBLEInstance *midi_ble_bluedroid_instance = nullptr;
}
} // namespace cs::midi_ble_bluedroid

using namespace cs;
using namespace cs::midi_ble_bluedroid;

void midi_ble_instance_handle_connect(uint16_t conn_handle) {
    if (midi_ble_bluedroid_instance)
        midi_ble_bluedroid_instance->handleConnect(
            BLEConnectionHandle {conn_handle});
}
void midi_ble_instance_handle_disconnect(uint16_t conn_handle) {
    if (midi_ble_bluedroid_instance)
        midi_ble_bluedroid_instance->handleDisconnect(
            BLEConnectionHandle {conn_handle});
}
void midi_ble_instance_handle_mtu(uint16_t conn_handle, uint16_t mtu) {
    if (midi_ble_bluedroid_instance)
        midi_ble_bluedroid_instance->handleMTU(
            BLEConnectionHandle {conn_handle}, mtu);
}
void midi_ble_instance_handle_subscribe(uint16_t conn_handle,
                                        uint16_t char_handle, bool notify) {
    if (midi_ble_bluedroid_instance)
        midi_ble_bluedroid_instance->handleSubscribe(
            BLEConnectionHandle {conn_handle},
            BLECharacteristicHandle {char_handle}, notify);
}
void midi_ble_instance_handle_data(uint16_t conn_handle, const uint8_t *data,
                                   uint16_t length) {
    if (midi_ble_bluedroid_instance) {
        BLEDataView view {data, length};
        auto data_gen = [view {view}]() mutable {
#if __cplusplus >= 201402
            return std::exchange(view, {});
#else
            auto ret = view;
            view = {};
            return ret;
#endif
        };
        midi_ble_bluedroid_instance->handleData(
            BLEConnectionHandle {conn_handle},
            BLEDataGenerator {compat::in_place, std::move(data_gen)},
            BLEDataLifetime::ConsumeImmediately);
    }
}

namespace cs::midi_ble_bluedroid {

bool init(MIDIBLEInstance &instance, BLESettings settings) {
    midi_ble_bluedroid_instance = &instance;
    set_midi_ble_name(settings.device_name);
    advertising_set_connection_interval(settings.connection_interval.minimum,
                                        settings.connection_interval.maximum);
    set_security_initiate_encryption(settings.initiate_security);
    return midi_init();
}

bool notify(BLEConnectionHandle conn_handle,
            BLECharacteristicHandle char_handle, BLEDataView data) {
    return midi_notify(conn_handle.conn, char_handle.characteristic, data.data,
                       data.length);
}

} // namespace cs::midi_ble_bluedroid

#endif
#endif