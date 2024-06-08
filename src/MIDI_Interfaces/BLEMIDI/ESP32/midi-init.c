#ifdef ESP32
#include <sdkconfig.h>
#if CONFIG_BT_BLE_ENABLED

/**
 * @file
 * 
 * Initialization of the Bluetooth stack and registration of the MIDI BLE
 * application.
 * 
 * @todo   Implement de-initialization, cleaning up everything we've configured.
 */

#include "events.h"
#include "midi-private.h"

#include <esp32-hal-bt.h>
#include <esp_bt_main.h>
#include <esp_gap_ble_api.h>
#include <esp_gatt_common_api.h>

#include <assert.h>

const char *midi_ble_name = "Control Surface (BLE)";

void set_midi_ble_name(const char *name) { midi_ble_name = name; }

bool midi_init(void) {
    if (!btStarted() && !btStart())
        return false;

    esp_err_t ret;

    esp_bluedroid_status_t bt_state = esp_bluedroid_get_status();
    if (bt_state == ESP_BLUEDROID_STATUS_UNINITIALIZED) {
        ret = esp_bluedroid_init();
        if (ret != ESP_OK) {
            ESP_LOGE("MIDIBLE", "Init bluetooth failed: %s",
                     esp_err_to_name(ret));
            return false;
        }
    }

    if (bt_state != ESP_BLUEDROID_STATUS_ENABLED) {
        ret = esp_bluedroid_enable();
        if (ret != ESP_OK) {
            ESP_LOGE("MIDIBLE", "Enable bluetooth failed: %s",
                     esp_err_to_name(ret));
            return false;
        }
    }

    ret = esp_ble_gatt_set_local_mtu(ESP_GATT_MAX_MTU_SIZE);
    if (ret != ESP_OK) {
        ESP_LOGE("MIDIBLE", "set local  MTU failed: %s", esp_err_to_name(ret));
        return false;
    }

    ret = esp_ble_gap_set_device_name(midi_ble_name);
    if (ret != ESP_OK) {
        ESP_LOGE("MIDIBLE", "set device name failed: %s", esp_err_to_name(ret));
        return false;
    }

    ret = esp_ble_gap_register_callback(gap_event_handler);
    if (ret != ESP_OK) {
        ESP_LOGE("MIDIBLE", "gap register error: %s", esp_err_to_name(ret));
        return false;
    }

    ret = esp_ble_gatts_register_callback(gatts_event_handler);
    if (ret != ESP_OK) {
        ESP_LOGE("MIDIBLE", "GATTS register error: %s", esp_err_to_name(ret));
        return false;
    }

    ret = esp_ble_gatts_app_register(midi_get_app_id());
    if (ret != ESP_OK) {
        ESP_LOGE("MIDIBLE", "GATTS app register error: %s",
                 esp_err_to_name(ret));
        return false;
    }

    // Authentication and security

    // Bonding with peer device after authentication
    esp_ble_auth_req_t auth_req = ESP_LE_AUTH_REQ_SC_MITM_BOND;
    // Set the IO capability to No output No input
    esp_ble_io_cap_t iocap = ESP_IO_CAP_NONE;
    uint8_t auth_option = ESP_BLE_ONLY_ACCEPT_SPECIFIED_AUTH_DISABLE;
    uint8_t key_size = 16;
    uint8_t init_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
    uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
    uint8_t oob_support = ESP_BLE_OOB_DISABLE;

    ret = esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req,
                                         sizeof(auth_req));
    if (ret != ESP_OK) {
        ESP_LOGE("MIDIBLE", "Failed to set ESP_BLE_SM_AUTHEN_REQ_MODE: %s",
                 esp_err_to_name(ret));
        return false;
    }

    ret = esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &iocap,
                                         sizeof(iocap));
    if (ret != ESP_OK) {
        ESP_LOGE("MIDIBLE", "Failed to set ESP_BLE_SM_IOCAP_MODE: %s",
                 esp_err_to_name(ret));
        return false;
    }

    ret = esp_ble_gap_set_security_param(
        ESP_BLE_SM_ONLY_ACCEPT_SPECIFIED_SEC_AUTH, &auth_option,
        sizeof(auth_option));
    if (ret != ESP_OK) {
        ESP_LOGE("MIDIBLE",
                 "Failed to set ESP_BLE_SM_ONLY_ACCEPT_SPECIFIED_SEC_AUTH: %s",
                 esp_err_to_name(ret));
        return false;
    }

    ret = esp_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE, &key_size,
                                         sizeof(key_size));
    if (ret != ESP_OK) {
        ESP_LOGE("MIDIBLE", "Failed to set ESP_BLE_SM_MAX_KEY_SIZE: %s",
                 esp_err_to_name(ret));
        return false;
    }

    ret = esp_ble_gap_set_security_param(ESP_BLE_SM_OOB_SUPPORT, &oob_support,
                                         sizeof(oob_support));
    if (ret != ESP_OK) {
        ESP_LOGE("MIDIBLE", "Failed to set ESP_BLE_SM_OOB_SUPPORT: %s",
                 esp_err_to_name(ret));
        return false;
    }

    ret = esp_ble_gap_set_security_param(ESP_BLE_SM_SET_INIT_KEY, &init_key,
                                         sizeof(init_key));
    if (ret != ESP_OK) {
        ESP_LOGE("MIDIBLE", "Failed to set ESP_BLE_SM_SET_INIT_KEY: %s",
                 esp_err_to_name(ret));
        return false;
    }

    ret = esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &rsp_key,
                                         sizeof(rsp_key));
    if (ret != ESP_OK) {
        ESP_LOGE("MIDIBLE", "Failed to set ESP_BLE_SM_SET_RSP_KEY: %s",
                 esp_err_to_name(ret));
        return false;
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);

    return true;
}

bool midi_deinit(void) { assert(!"Not implemented"); }

#endif
#endif