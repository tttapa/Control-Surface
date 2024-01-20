#pragma once

#include "../BLEAPI.hpp"

namespace cs::midi_ble_nimble {

bool init(MIDIBLEInstance &instance, BLESettings ble_settings);
bool notify(BLEConnectionHandle conn_handle,
            BLECharacteristicHandle char_handle, BLEDataView data);

} // namespace cs::midi_ble_nimble

#include "app.ipp"
