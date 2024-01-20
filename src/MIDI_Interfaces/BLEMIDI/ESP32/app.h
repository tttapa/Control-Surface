#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void midi_ble_instance_handle_connect(uint16_t conn_handle);
void midi_ble_instance_handle_disconnect(uint16_t conn_handle);
void midi_ble_instance_handle_mtu(uint16_t conn_handle, uint16_t mtu);
void midi_ble_instance_handle_subscribe(uint16_t conn_handle,
                                        uint16_t char_handle, bool notify);
void midi_ble_instance_handle_data(uint16_t conn_handle, const uint8_t *data,
                                   uint16_t length);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include "../BLEAPI.hpp"

namespace cs::midi_ble_bluedroid {

bool init(MIDIBLEInstance &instance, BLESettings settings);
bool notify(BLEConnectionHandle conn_handle,
            BLECharacteristicHandle char_handle, BLEDataView data);

} // namespace cs::midi_ble_bluedroid

#endif