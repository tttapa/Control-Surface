#pragma once

#include "../BLEAPI.hpp"

namespace cs::midi_ble_btstack {

bool init(MIDIBLEInstance &instance, BLESettings settings);
void notify(BLEConnectionHandle conn_handle,
            BLECharacteristicHandle char_handle, BLEDataView data);

} // namespace cs::midi_ble_btstack
