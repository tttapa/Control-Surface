#ifdef ESP32
#include <sdkconfig.h>
#if CONFIG_BT_BLE_ENABLED

/**
 * @file 
 * 
 * Handler for BLE MIDI write events. These events occur when the client sends
 * us MIDI data.
 */

#include "app.h"
#include "logging.h"
#include "midi-private.h"

#include <esp_gatt_common_api.h>

void midi_handle_write_event(esp_gatt_if_t gatts_if,
                             esp_ble_gatts_cb_param_t *param) {
    if (param->write.need_rsp) {
        // MIDI never needs a response, send an error response
        esp_ble_gatts_send_response(gatts_if, param->write.conn_id,
                                    param->write.trans_id,
                                    ESP_GATT_REQ_NOT_SUPPORTED, NULL);
        return;
    }
    // Not a prepare event, handle the data immediately
    midi_ble_instance_handle_data(param->write.conn_id, param->write.value,
                                  param->write.len);
}

void midi_handle_write_exec_event(esp_gatt_if_t gatts_if,
                                  esp_ble_gatts_cb_param_t *param) {
    esp_ble_gatts_send_response(gatts_if, param->exec_write.conn_id,
                                param->exec_write.trans_id,
                                ESP_GATT_REQ_NOT_SUPPORTED, NULL);
}

#endif
#endif