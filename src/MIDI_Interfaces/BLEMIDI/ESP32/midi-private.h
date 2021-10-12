#pragma once

/**
 * @file 
 * 
 * Declarations of internal functions for the MIDI over BLE system, used in the
 * midi-*.c files.
 */

#include "midi.h"
#include <esp_gatt_defs.h>
#include <esp_gatts_api.h>

void midi_handle_gatts_event(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                             esp_ble_gatts_cb_param_t *param);

void midi_handle_register_app_event(esp_gatt_if_t gatts_if,
                                    esp_ble_gatts_cb_param_t *param);
void midi_handle_create_attribute_table_event(esp_gatt_if_t gatts_if,
                                              esp_ble_gatts_cb_param_t *param);
void midi_handle_connect_event(esp_gatt_if_t gatts_if,
                               esp_ble_gatts_cb_param_t *param);
void midi_handle_disconnect_event(esp_gatt_if_t gatts_if,
                                  esp_ble_gatts_cb_param_t *param);
void midi_handle_write_event(esp_gatt_if_t gatts_if,
                             esp_ble_gatts_cb_param_t *param);
void midi_handle_write_exec_event(esp_gatt_if_t gatts_if,
                                  esp_ble_gatts_cb_param_t *param);
void midi_handle_mtu_event(esp_gatt_if_t gatts_if,
                           esp_ble_gatts_cb_param_t *param);
void midi_handle_read_event(esp_gatt_if_t gatts_if,
                            esp_ble_gatts_cb_param_t *param);

uint16_t midi_get_service_handle(void);
uint16_t midi_get_characteristic_handle(void);
uint16_t midi_get_descriptor_handle(void);

void midi_register_interface(esp_gatt_if_t gatts_if);
uint16_t midi_get_app_id(void);
void midi_set_connection_id(uint16_t conn_id);
uint16_t midi_get_connection_id(void);
uint16_t midi_get_gatts_if(void);
