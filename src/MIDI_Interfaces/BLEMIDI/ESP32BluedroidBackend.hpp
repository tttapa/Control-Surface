#pragma once

#include "ESP32/app.h"
#include "ESP32Backend.hpp"

BEGIN_CS_NAMESPACE

namespace ble_backend {
struct ESP32BluedroidBLE {
    static constexpr auto notify = midi_ble_bluedroid::notify;
    static constexpr auto init = midi_ble_bluedroid::init;
};
} // namespace ble_backend

/// ESP32 Bluedroid backend intended to be plugged into
/// @ref GenericBLEMIDI_Interface.
using ESP32BluedroidBackend = ESP32BLEBackend<ble_backend::ESP32BluedroidBLE>;

END_CS_NAMESPACE