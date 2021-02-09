#ifdef ESP32

#include "advertising.h"
#include "esp_enums2string.h"
#include "events-debug.h"
#include "logging.h"
#include "midi-private.h"

#include <string.h>

void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                         esp_ble_gatts_cb_param_t *param) {
    print_gatts_event(event, gatts_if, param);

    // Upon registration, associate the interface id with the registered app
    if (event == ESP_GATTS_REG_EVT && param->reg.app_id == midi_get_app_id()) {
        ESP_LOGI("MIDIBLE", "Registering MIDI app");
        if (param->reg.status != ESP_GATT_OK) {
            ESP_LOGE("MIDIBLE", "Register app failed, app_id %04x, status: %d",
                     param->reg.app_id, param->reg.status);
            return;
        }
        midi_register_interface(gatts_if);
    }

    midi_handle_gatts_event(event, gatts_if, param);
}

void gap_event_handler(esp_gap_ble_cb_event_t event,
                       esp_ble_gap_cb_param_t *param) {
    print_gap_event(event, param);

    switch (event) {
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
            advertising_handle_config_complete_event(param);
            break;
        case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
            advertising_handle_config_response_complete_event(param);
            break;
        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
            if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
                ESP_LOGE("MIDIBLE", "Advertising start failed");
            }
            break;
        case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
            if (param->adv_stop_cmpl.status != ESP_BT_STATUS_SUCCESS) {
                ESP_LOGE("MIDIBLE", "Advertising stop failed");
            }
            break;

        case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT: break;

        case ESP_GAP_BLE_PASSKEY_REQ_EVT: break;
        case ESP_GAP_BLE_OOB_REQ_EVT: break;
        case ESP_GAP_BLE_LOCAL_IR_EVT: break;
        case ESP_GAP_BLE_LOCAL_ER_EVT: break;
        case ESP_GAP_BLE_NC_REQ_EVT: break;
        case ESP_GAP_BLE_SEC_REQ_EVT:
            // Send the positive (true) security response to the peer device to
            // accept the security request. If not accept the security request,
            // should send the security response with negative (false) accept
            // value.
            esp_ble_gap_security_rsp(param->ble_security.ble_req.bd_addr, true);
            break;
        case ESP_GAP_BLE_PASSKEY_NOTIF_EVT: break;
        case ESP_GAP_BLE_KEY_EVT: break;
        case ESP_GAP_BLE_AUTH_CMPL_EVT: break;
        case ESP_GAP_BLE_REMOVE_BOND_DEV_COMPLETE_EVT: break;

        case ESP_GAP_BLE_SET_LOCAL_PRIVACY_COMPLETE_EVT:
            if (param->local_privacy_cmpl.status != ESP_BT_STATUS_SUCCESS) {
                ESP_LOGE("MIDIBLE",
                         "config local privacy failed, error status = %d",
                         param->local_privacy_cmpl.status);
                break;
            }
            advertising_config();

        default: break;
    }
}

#endif