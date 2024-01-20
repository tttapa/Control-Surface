#ifdef ESP32
#include <sdkconfig.h>
#if CONFIG_BT_BLE_ENABLED

/**
 * @file
 * 
 * Handling MTU (maximum transmission unit) negotiation events for MIDI over 
 * BLE.
 */

#include "app.h"
#include "logging.h"
#include "midi-private.h"

void midi_handle_mtu_event(esp_gatt_if_t gatts_if,
                           esp_ble_gatts_cb_param_t *param) {
    uint16_t midi_mtu = param->mtu.mtu;
    ESP_LOGI("MIDIBLE", "MTU: %d", midi_mtu);
    midi_ble_instance_handle_mtu(param->mtu.conn_id, midi_mtu);
}

#endif
#endif