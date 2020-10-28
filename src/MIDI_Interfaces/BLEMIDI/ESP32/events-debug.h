#pragma once

/**
 * @file 
 * 
 * Helpers for printing Bluetooth events and status.
 */

#include <esp_gap_ble_api.h>
#include <esp_gatt_defs.h>
#include <esp_gatts_api.h>

void show_bonded_devices(void);
void print_gatts_event(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                       esp_ble_gatts_cb_param_t *param);
void print_gap_event(esp_gap_ble_cb_event_t event,
                     esp_ble_gap_cb_param_t *param);
