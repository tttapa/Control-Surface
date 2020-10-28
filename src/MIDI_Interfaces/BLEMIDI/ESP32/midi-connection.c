#ifdef ESP32

/**
 * @file
 * 
 * Handling of Bluetooth Low Energy connect and disconnect events.
 */

#include "logging.h"
#include "midi-private.h"
#include "advertising.h"

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

    midi_set_connection_id(param->connect.conn_id);

    // <?> Why do we need to update the connection parameters?
    //     How are these parameters different from the advertising
    //     parameters?

    // For the IOS system, please reference the apple official documents about
    // the ble connection parameters restrictions:
    // https://developer.apple.com/accessories/Accessory-Design-Guidelines.pdf
    // https://developer.apple.com/library/archive/qa/qa1931/_index.html

    esp_ble_conn_update_params_t conn_params;
    memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
    conn_params.max_int = 0x000C;
    conn_params.min_int = 0x000C;
    conn_params.latency = 0;
    conn_params.timeout = 400; // timeout = 400*10ms = 4s
    esp_ble_gap_update_conn_params(&conn_params);
}

void midi_handle_disconnect_event(esp_gatt_if_t gatts_if,
                                  esp_ble_gatts_cb_param_t *param) {
    ESP_LOGI("MIDIBLE", "Disconnect reason: %d", param->disconnect.reason);

    midi_set_connection_id(0);

    advertising_config();
}

#endif