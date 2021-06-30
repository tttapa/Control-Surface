#ifdef ESP32

#include "esp_enums2string.h"

const char *esp_gatts_cb_event_to_string(esp_gatts_cb_event_t event) {
    switch (event) {
        case ESP_GATTS_REG_EVT: return "ESP_GATTS_REG_EVT";
        case ESP_GATTS_READ_EVT: return "ESP_GATTS_READ_EVT";
        case ESP_GATTS_WRITE_EVT: return "ESP_GATTS_WRITE_EVT";
        case ESP_GATTS_EXEC_WRITE_EVT: return "ESP_GATTS_EXEC_WRITE_EVT";
        case ESP_GATTS_MTU_EVT: return "ESP_GATTS_MTU_EVT";
        case ESP_GATTS_CONF_EVT: return "ESP_GATTS_CONF_EVT";
        case ESP_GATTS_UNREG_EVT: return "ESP_GATTS_UNREG_EVT";
        case ESP_GATTS_CREATE_EVT: return "ESP_GATTS_CREATE_EVT";
        case ESP_GATTS_ADD_INCL_SRVC_EVT: return "ESP_GATTS_ADD_INCL_SRVC_EVT";
        case ESP_GATTS_ADD_CHAR_EVT: return "ESP_GATTS_ADD_CHAR_EVT";
        case ESP_GATTS_ADD_CHAR_DESCR_EVT:
            return "ESP_GATTS_ADD_CHAR_DESCR_EVT";
        case ESP_GATTS_DELETE_EVT: return "ESP_GATTS_DELETE_EVT";
        case ESP_GATTS_START_EVT: return "ESP_GATTS_START_EVT";
        case ESP_GATTS_STOP_EVT: return "ESP_GATTS_STOP_EVT";
        case ESP_GATTS_CONNECT_EVT: return "ESP_GATTS_CONNECT_EVT";
        case ESP_GATTS_DISCONNECT_EVT: return "ESP_GATTS_DISCONNECT_EVT";
        case ESP_GATTS_OPEN_EVT: return "ESP_GATTS_OPEN_EVT";
        case ESP_GATTS_CANCEL_OPEN_EVT: return "ESP_GATTS_CANCEL_OPEN_EVT";
        case ESP_GATTS_CLOSE_EVT: return "ESP_GATTS_CLOSE_EVT";
        case ESP_GATTS_LISTEN_EVT: return "ESP_GATTS_LISTEN_EVT";
        case ESP_GATTS_CONGEST_EVT: return "ESP_GATTS_CONGEST_EVT";
        case ESP_GATTS_RESPONSE_EVT: return "ESP_GATTS_RESPONSE_EVT";
        case ESP_GATTS_CREAT_ATTR_TAB_EVT:
            return "ESP_GATTS_CREAT_ATTR_TAB_EVT";
        case ESP_GATTS_SET_ATTR_VAL_EVT: return "ESP_GATTS_SET_ATTR_VAL_EVT";
        case ESP_GATTS_SEND_SERVICE_CHANGE_EVT:
            return "ESP_GATTS_SEND_SERVICE_CHANGE_EVT";

        default: return "<invalid>";
    }
}

const char *esp_ble_key_type_to_string(esp_ble_key_type_t key_type) {
    switch (key_type) {
        case ESP_LE_KEY_NONE: return "ESP_LE_KEY_NONE";
        case ESP_LE_KEY_PENC: return "ESP_LE_KEY_PENC";
        case ESP_LE_KEY_PID: return "ESP_LE_KEY_PID";
        case ESP_LE_KEY_PCSRK: return "ESP_LE_KEY_PCSRK";
        case ESP_LE_KEY_PLK: return "ESP_LE_KEY_PLK";
        case ESP_LE_KEY_LLK: return "ESP_LE_KEY_LLK";
        case ESP_LE_KEY_LENC: return "ESP_LE_KEY_LENC";
        case ESP_LE_KEY_LID: return "ESP_LE_KEY_LID";
        case ESP_LE_KEY_LCSRK: return "ESP_LE_KEY_LCSRK";
        default: return "<invalid>";
    }
}

const char *esp_ble_auth_req_to_string(esp_ble_auth_req_t auth_type) {
    switch (auth_type) {
        case ESP_LE_AUTH_NO_BOND: return "ESP_LE_AUTH_NO_BOND";
        case ESP_LE_AUTH_BOND: return "ESP_LE_AUTH_BOND";
        case ESP_LE_AUTH_REQ_MITM: return "ESP_LE_AUTH_REQ_MITM";
        case ESP_LE_AUTH_REQ_SC_ONLY: return "ESP_LE_AUTH_REQ_SC_ONLY";
        case ESP_LE_AUTH_REQ_SC_BOND: return "ESP_LE_AUTH_REQ_SC_BOND";
        case ESP_LE_AUTH_REQ_SC_MITM: return "ESP_LE_AUTH_REQ_SC_MITM";
        case ESP_LE_AUTH_REQ_SC_MITM_BOND:
            return "ESP_LE_AUTH_REQ_SC_MITM_BOND";
        default: return "<invalid>";
    }
}

const char *esp_gap_ble_cb_event_to_string(esp_gap_ble_cb_event_t event) {
    switch (event) {
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
            return "ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT";
        case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
            return "ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT";
        case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT:
            return "ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT";
        case ESP_GAP_BLE_SCAN_RESULT_EVT: return "ESP_GAP_BLE_SCAN_RESULT_EVT";
        case ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT:
            return "ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT";
        case ESP_GAP_BLE_SCAN_RSP_DATA_RAW_SET_COMPLETE_EVT:
            return "ESP_GAP_BLE_SCAN_RSP_DATA_RAW_SET_COMPLETE_EVT";
        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
            return "ESP_GAP_BLE_ADV_START_COMPLETE_EVT";
        case ESP_GAP_BLE_SCAN_START_COMPLETE_EVT:
            return "ESP_GAP_BLE_SCAN_START_COMPLETE_EVT";
        case ESP_GAP_BLE_AUTH_CMPL_EVT: return "ESP_GAP_BLE_AUTH_CMPL_EVT";
        case ESP_GAP_BLE_KEY_EVT: return "ESP_GAP_BLE_KEY_EVT";
        case ESP_GAP_BLE_SEC_REQ_EVT: return "ESP_GAP_BLE_SEC_REQ_EVT";
        case ESP_GAP_BLE_PASSKEY_NOTIF_EVT:
            return "ESP_GAP_BLE_PASSKEY_NOTIF_EVT";
        case ESP_GAP_BLE_PASSKEY_REQ_EVT: return "ESP_GAP_BLE_PASSKEY_REQ_EVT";
        case ESP_GAP_BLE_OOB_REQ_EVT: return "ESP_GAP_BLE_OOB_REQ_EVT";
        case ESP_GAP_BLE_LOCAL_IR_EVT: return "ESP_GAP_BLE_LOCAL_IR_EVT";
        case ESP_GAP_BLE_LOCAL_ER_EVT: return "ESP_GAP_BLE_LOCAL_ER_EVT";
        case ESP_GAP_BLE_NC_REQ_EVT: return "ESP_GAP_BLE_NC_REQ_EVT";
        case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
            return "ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT";
        case ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT:
            return "ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT";
        case ESP_GAP_BLE_SET_STATIC_RAND_ADDR_EVT:
            return "ESP_GAP_BLE_SET_STATIC_RAND_ADDR_EVT";
        case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
            return "ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT";
        case ESP_GAP_BLE_SET_PKT_LENGTH_COMPLETE_EVT:
            return "ESP_GAP_BLE_SET_PKT_LENGTH_COMPLETE_EVT";
        case ESP_GAP_BLE_SET_LOCAL_PRIVACY_COMPLETE_EVT:
            return "ESP_GAP_BLE_SET_LOCAL_PRIVACY_COMPLETE_EVT";
        case ESP_GAP_BLE_REMOVE_BOND_DEV_COMPLETE_EVT:
            return "ESP_GAP_BLE_REMOVE_BOND_DEV_COMPLETE_EVT";
        case ESP_GAP_BLE_CLEAR_BOND_DEV_COMPLETE_EVT:
            return "ESP_GAP_BLE_CLEAR_BOND_DEV_COMPLETE_EVT";
        case ESP_GAP_BLE_GET_BOND_DEV_COMPLETE_EVT:
            return "ESP_GAP_BLE_GET_BOND_DEV_COMPLETE_EVT";
        case ESP_GAP_BLE_READ_RSSI_COMPLETE_EVT:
            return "ESP_GAP_BLE_READ_RSSI_COMPLETE_EVT";
        case ESP_GAP_BLE_UPDATE_WHITELIST_COMPLETE_EVT:
            return "ESP_GAP_BLE_UPDATE_WHITELIST_COMPLETE_EVT";
        case ESP_GAP_BLE_UPDATE_DUPLICATE_EXCEPTIONAL_LIST_COMPLETE_EVT:
            return "ESP_GAP_BLE_UPDATE_DUPLICATE_EXCEPTIONAL_LIST_COMPLETE_EVT";
        case ESP_GAP_BLE_EVT_MAX: return "ESP_GAP_BLE_EVT_MAX";
        default: return "<invalid>";
    }
}

#endif