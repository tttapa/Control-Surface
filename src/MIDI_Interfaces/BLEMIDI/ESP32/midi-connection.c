#ifdef ESP32
#include <sdkconfig.h>
#if CONFIG_BT_BLE_ENABLED

/**
 * @file
 * 
 * Handling of Bluetooth Low Energy connect and disconnect events.
 */

#include "advertising.h"
#include "app.h"
#include "logging.h"
#include "midi-private.h"

#include <string.h> // memcpy

#include <esp_gap_ble_api.h>

void midi_handle_connect_event(esp_gatt_if_t gatts_if,
                               esp_ble_gatts_cb_param_t *param) {
    ESP_LOGI("MIDIBLE",
             "Connection ID: %d, remote: "
             "%02x:%02x:%02x:%02x:%02x:%02x:",
             param->connect.conn_id, param->connect.remote_bda[0],
             param->connect.remote_bda[1], param->connect.remote_bda[2],
             param->connect.remote_bda[3], param->connect.remote_bda[4],
             param->connect.remote_bda[5]);

    midi_ble_instance_handle_connect(param->connect.conn_id);

    // <?> Do we need to update the connection parameters?
    //     Can we just rely on the central picking sensible defaults or
    //     honoring the advertising connection interval range?

    // For the IOS system, please reference the apple official documents about
    // the ble connection parameters restrictions:
    // https://developer.apple.com/accessories/Accessory-Design-Guidelines.pdf
    // https://developer.apple.com/library/archive/qa/qa1931/_index.html

    esp_ble_conn_update_params_t conn_params;
    memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
    advertising_get_connection_interval(&conn_params.min_int,
                                        &conn_params.max_int);
    conn_params.latency = 0;
    conn_params.timeout = 400; // timeout = 400*10ms = 4s
    esp_ble_gap_update_conn_params(&conn_params);
}

void midi_handle_disconnect_event(esp_gatt_if_t gatts_if,
                                  esp_ble_gatts_cb_param_t *param) {
    ESP_LOGI("MIDIBLE", "Disconnect reason: %d", param->disconnect.reason);
    midi_ble_instance_handle_disconnect(param->disconnect.conn_id);
    // Start advertising again
    advertising_config();
}

#endif
#endif