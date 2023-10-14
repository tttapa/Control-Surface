#ifdef ESP32
#include <sdkconfig.h>
#if CONFIG_BT_BLE_ENABLED

/**
 * @file
 * 
 * Handling MTU (maximum transmission unit) negotiation events for MIDI over 
 * BLE.
 */

#include "logging.h"
#include "midi-private.h"

static midi_mtu_callback_t midi_mtu_callback = NULL;
static uint16_t midi_mtu = 0;

void midi_set_mtu_callback(midi_mtu_callback_t cb) { midi_mtu_callback = cb; }

void midi_handle_mtu_event(esp_gatt_if_t gatts_if,
                           esp_ble_gatts_cb_param_t *param) {
    midi_mtu = param->mtu.mtu;
    ESP_LOGI("MIDIBLE", "MTU: %d", midi_mtu);
    if (midi_mtu_callback) {
        midi_mtu_callback(midi_mtu);
    }
}

uint16_t midi_get_mtu(void) { return midi_mtu; }

#endif
#endif