#ifdef ESP32

/** 
 * @file
 * 
 * Sending BLE MIDI notify packets to send MIDI data to the client.
 */

#include "midi-private.h"
#include "ble2902.h"

bool midi_notify(const uint8_t *data, size_t length) {
    if (midi_get_gatts_if() == ESP_GATT_IF_NONE)
        return false;
    if (!ble2902_notifications_enabled(midi_get_descriptor_handle()))
        return false;
    if (length > midi_get_mtu() - 3)
        return false;
    esp_err_t ret = esp_ble_gatts_send_indicate(
        midi_get_gatts_if(), midi_get_connection_id(),
        midi_get_characteristic_handle(), length, (uint8_t *)data, false);
    return ret == ESP_OK;
}

#endif