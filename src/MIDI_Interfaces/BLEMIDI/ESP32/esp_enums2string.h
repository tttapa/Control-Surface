#pragma once

/**
 * @file 
 * 
 * Helpers for printing the names of enum values of the ESP32 Bluetooth API.
 */

#include <esp_gap_ble_api.h>
#include <esp_gatts_api.h>

#ifdef __cplusplus
extern "C" {
#endif

const char *esp_gatts_cb_event_to_string(esp_gatts_cb_event_t event);
const char *esp_ble_key_type_to_string(esp_ble_key_type_t key_type);
const char *esp_ble_auth_req_to_string(esp_ble_auth_req_t auth_type);
const char *esp_gap_ble_cb_event_to_string(esp_gap_ble_cb_event_t event);

#ifdef __cplusplus
}
#endif
