#pragma once

#include "ble.h"

#if CS_MIDI_BLE_ESP_IDF_NIMBLE
#include <host/ble_uuid.h>
#else
#include <nimble/nimble/host/include/host/ble_uuid.h>
#endif

#include "ble-macro-fix.h"

extern const ble_uuid128_t midi_ble_service_uuid;
extern const ble_uuid128_t midi_ble_characteristic_uuid;

#ifdef __cplusplus
extern "C" {
#endif

const struct ble_gatt_svc_def *midi_ble_get_service(void);

#ifdef __cplusplus
}
#endif

#include "gatt.c.ipp"
