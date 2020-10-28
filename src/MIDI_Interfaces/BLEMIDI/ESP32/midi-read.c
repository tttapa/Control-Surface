#ifdef ESP32

/** 
 * @file
 * 
 * Handler for MIDI BLE read events.
 * 
 * @note    This doesn't read MIDI data, it always responds with no payload, 
 *          as mandated by the MIDI BLE spec.
 */

#include "midi-private.h"

#include <string.h> // memset

void midi_handle_read_event(esp_gatt_if_t gatts_if,
                            esp_ble_gatts_cb_param_t *param) {
    // MIDI should respond with no data
    // TODO: over 600 bytes, do we want this on the stack?
    esp_gatt_rsp_t rsp;
    memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
    rsp.attr_value.handle = param->read.handle;
    // Respond without data
    rsp.attr_value.len = 0;
    esp_ble_gatts_send_response(gatts_if, param->read.conn_id,
                                param->read.trans_id, ESP_GATT_OK, &rsp);
}

#endif