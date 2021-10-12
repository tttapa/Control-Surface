#ifdef ESP32

#include "events-debug.h"
#include "esp_enums2string.h"
#include "logging.h"

#include <stdlib.h> // malloc

void print_gatts_event(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                       esp_ble_gatts_cb_param_t *param) {
    ESP_LOGI("MIDIBLE", "GATTS event: %s", esp_gatts_cb_event_to_string(event));
}

void show_bonded_devices(void) {
    int dev_num = esp_ble_get_bond_device_num();
    esp_ble_bond_dev_t *dev_list =
        (esp_ble_bond_dev_t *)malloc(sizeof(esp_ble_bond_dev_t) * dev_num);
    esp_ble_get_bond_device_list(&dev_num, dev_list);
    ESP_LOGI("MIDIBLE", "Bonded devices list [%d]:", dev_num);
    for (int i = 0; i < dev_num; i++) {
        ESP_LOGI("MIDIBLE", "  - %02x:%02x:%02x:%02x:%02x:%02x",
                 dev_list[i].bd_addr[0], dev_list[i].bd_addr[1],
                 dev_list[i].bd_addr[2], dev_list[i].bd_addr[3],
                 dev_list[i].bd_addr[4], dev_list[i].bd_addr[5]);
    }
    free(dev_list);
}

void print_gap_event(esp_gap_ble_cb_event_t event,
                     esp_ble_gap_cb_param_t *param) {
    ESP_LOGI("MIDIBLE", "GAP event: %s", esp_gap_ble_cb_event_to_string(event));

    switch (event) {
        case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
            ESP_LOGI("MIDIBLE",
                     "status: %d, min_int: %d, max_int: %d, conn_int: %d, "
                     "latency: %d, timeout: %d",
                     param->update_conn_params.status,
                     param->update_conn_params.min_int,
                     param->update_conn_params.max_int,
                     param->update_conn_params.conn_int,
                     param->update_conn_params.latency,
                     param->update_conn_params.timeout);
            break;

        case ESP_GAP_BLE_PASSKEY_NOTIF_EVT:
            // The app will receive this evt when the IO  has Output capability
            // and the peer device IO has Input capability.
            // Show the passkey number to the user to input on the peer device.
            ESP_LOGI("MIDIBLE", "The passkey Notify number: %06d",
                     param->ble_security.key_notif.passkey);
            break;

        case ESP_GAP_BLE_KEY_EVT:
            ESP_LOGI("MIDIBLE", "key type: %s",
                     esp_ble_key_type_to_string(
                         param->ble_security.ble_key.key_type));
            break;

        case ESP_GAP_BLE_AUTH_CMPL_EVT:
            ESP_LOGI("MIDIBLE", "%02x:%02x:%02x:%02x:%02x:%02x",
                     param->ble_security.auth_cmpl.bd_addr[0],
                     param->ble_security.auth_cmpl.bd_addr[1],
                     param->ble_security.auth_cmpl.bd_addr[2],
                     param->ble_security.auth_cmpl.bd_addr[3],
                     param->ble_security.auth_cmpl.bd_addr[4],
                     param->ble_security.auth_cmpl.bd_addr[5]);
            ESP_LOGI("MIDIBLE", "address type: %d",
                     param->ble_security.auth_cmpl.addr_type);
            ESP_LOGI("MIDIBLE", "pair status: %s",
                     param->ble_security.auth_cmpl.success ? "success"
                                                           : "fail");
            if (!param->ble_security.auth_cmpl.success) {
                ESP_LOGI("MIDIBLE", "fail reason: 0x%x",
                         param->ble_security.auth_cmpl.fail_reason);
            } else {
                ESP_LOGI("MIDIBLE", "auth mode: %s",
                         esp_ble_auth_req_to_string(
                             param->ble_security.auth_cmpl.auth_mode));
            }
            show_bonded_devices();
            break;

        case ESP_GAP_BLE_REMOVE_BOND_DEV_COMPLETE_EVT:
            ESP_LOGI("MIDIBLE", "status: %d",
                     param->remove_bond_dev_cmpl.status);
            ESP_LOGI("MIDIBLE", "%02x:%02x:%02x:%02x:%02x:%02x",
                     param->remove_bond_dev_cmpl.bd_addr[0],
                     param->remove_bond_dev_cmpl.bd_addr[1],
                     param->remove_bond_dev_cmpl.bd_addr[2],
                     param->remove_bond_dev_cmpl.bd_addr[3],
                     param->remove_bond_dev_cmpl.bd_addr[4],
                     param->remove_bond_dev_cmpl.bd_addr[5]);
            break;

        default: break;
    }
}

#endif