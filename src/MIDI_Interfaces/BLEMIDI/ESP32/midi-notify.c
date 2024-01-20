#ifdef ESP32
#include <sdkconfig.h>
#if CONFIG_BT_BLE_ENABLED

/** 
 * @file
 * 
 * Sending BLE MIDI notify packets to send MIDI data to the client.
 */

#include "ble2902.h"
#include "midi-private.h"

bool midi_notify(uint16_t conn_handle, uint16_t char_handle,
                 const uint8_t *data, size_t length) {
    if (midi_get_gatts_if() == ESP_GATT_IF_NONE)
        return false;
    esp_err_t ret = esp_ble_gatts_send_indicate(midi_get_gatts_if(),
                                                conn_handle, char_handle,
                                                length, (uint8_t *)data, false);
    return ret == ESP_OK;
}

#endif
#endif