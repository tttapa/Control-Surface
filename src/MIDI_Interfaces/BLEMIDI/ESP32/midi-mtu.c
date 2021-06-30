#ifdef ESP32

/**
 * @file
 * 
 * Handling MTU (maximum transmission unit) negotiation events for MIDI over 
 * BLE.
 */

#include "midi-private.h"
#include "logging.h"

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