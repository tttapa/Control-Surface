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

#ifdef __cplusplus
extern "C" {
#endif

int cs_midi_ble_characteristic_callback(uint16_t conn_handle,
                                        uint16_t attr_handle,
                                        struct ble_gatt_access_ctxt *ctxt,
                                        void *arg);
void cs_midi_ble_service_register_callback(struct ble_gatt_register_ctxt *ctxt,
                                           void *arg);
int cs_midi_ble_gap_callback(struct ble_gap_event *event, void *arg);
void cs_midi_ble_on_sync(void);
void cs_midi_ble_on_reset(int reason);

#ifdef __cplusplus
} // extern "C"
#endif
