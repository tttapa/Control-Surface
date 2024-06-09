#pragma once

#include "ble.h"

#if CS_MIDI_BLE_ESP_IDF_NIMBLE
#include <host/ble_gap.h>
#include <host/ble_gatt.h>
#else
#include <nimble/nimble/host/include/host/ble_gap.h>
#include <nimble/nimble/host/include/host/ble_gatt.h>
#endif

#include "ble-macro-fix.h"

#include "../BLEAPI.hpp"

namespace cs::midi_ble_nimble {

struct MIDIBLEState {
    constexpr static uint16_t invalid_handle = 0xFFFF;
    MIDIBLEInstance *instance = nullptr;
    uint16_t midi_characteristic_handle = invalid_handle;
    uint8_t address_type = 0;
    BLESettings settings;
};

extern MIDIBLEState *state;

} // namespace cs::midi_ble_nimble

#include "events.ipp"
