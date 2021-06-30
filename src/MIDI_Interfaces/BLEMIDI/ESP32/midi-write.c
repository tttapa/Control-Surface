#ifdef ESP32

/**
 * @file 
 * 
 * Handler for BLE MIDI write events. These events occur when the client sends
 * us MIDI data.
 * 
 * @todo    This file also implements long writes (prepare write/execute write)
 *          events. This hasn't been tested, since it seems like most MIDI BLE
 *          clients use normal write packets only.
 */

#include "logging.h"
#include "midi-private.h"

#include <esp_gatt_common_api.h>

#include <stddef.h>
#include <stdlib.h> // malloc
#include <string.h> // memcpy

typedef struct {
    uint8_t *prepare_buf;
    int prepare_len;
} prepare_type_env_t;
static prepare_type_env_t MIDI_prepare_write_env;
const static size_t PREPARE_BUF_MAX_SIZE = ESP_GATT_MAX_MTU_SIZE;

static midi_write_callback_t midi_callback = NULL;

void midi_set_write_callback(midi_write_callback_t cb) { midi_callback = cb; }

void midi_handle_write_event(esp_gatt_if_t gatts_if,
                             esp_ble_gatts_cb_param_t *param) {
    if (param->write.need_rsp)
        return; // MIDI never needs response, return without acknowledgement

    // If this is a long write across multiple packets
    if (param->write.is_prep) {
        esp_gatt_status_t status = ESP_GATT_OK;
        // Allocate a buffer to receive the write data
        if (MIDI_prepare_write_env.prepare_buf == NULL) {
            MIDI_prepare_write_env.prepare_buf =
                (uint8_t *)malloc(PREPARE_BUF_MAX_SIZE * sizeof(uint8_t));
            MIDI_prepare_write_env.prepare_len = 0;
            if (MIDI_prepare_write_env.prepare_buf == NULL) {
                ESP_LOGE("MIDIBLE", "Gatt_server prep no mem\n");
                status = ESP_GATT_NO_RESOURCES;
            }
        }
        // Make sure that the data fits in the buffer
        if (param->write.offset > PREPARE_BUF_MAX_SIZE) {
            status = ESP_GATT_INVALID_OFFSET;
        }
        if ((param->write.offset + param->write.len) > PREPARE_BUF_MAX_SIZE) {
            status = ESP_GATT_INVALID_ATTR_LEN;
        }

        // If we're either out of buffer space or out of memory, don't store the
        // data
        if (status != ESP_GATT_OK) {
            // We don't need to send a response for MIDI
            return;
        }

        // Copy the write data to the buffer
        memcpy(MIDI_prepare_write_env.prepare_buf + param->write.offset,
               param->write.value, param->write.len);
        MIDI_prepare_write_env.prepare_len += param->write.len;
    } else {
        // Not a prepare event, handle the data immediately
        if (midi_callback)
            midi_callback(param->write.value, param->write.len);
    }
}

// Logs prepared written data and frees the buffer
void midi_handle_write_exec_event(esp_gatt_if_t gatts_if,
                                  esp_ble_gatts_cb_param_t *param) {
    if (param->exec_write.exec_write_flag == ESP_GATT_PREP_WRITE_CANCEL) {
        ESP_LOGI("MIDIBLE", "ESP_GATT_PREP_WRITE_CANCEL");
    }

    if (midi_callback && MIDI_prepare_write_env.prepare_buf &&
        MIDI_prepare_write_env.prepare_len)
        midi_callback(MIDI_prepare_write_env.prepare_buf,
                      MIDI_prepare_write_env.prepare_len);

    // if (*MIDI_char_handle == param->exec_write.handle) {
    // TODO: how do we know the write was for this characteristic?
    // Check transfer ID? Connection ID?
    if (MIDI_prepare_write_env.prepare_buf) {
        free(MIDI_prepare_write_env.prepare_buf);
        MIDI_prepare_write_env.prepare_buf = NULL;
    }
    MIDI_prepare_write_env.prepare_len = 0;

    esp_ble_gatts_send_response(gatts_if, param->exec_write.conn_id,
                                param->exec_write.trans_id, ESP_GATT_OK, NULL);
}

#endif