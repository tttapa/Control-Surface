#ifdef ESP32
#include <sdkconfig.h>
#if CONFIG_BT_BLE_ENABLED

#include "advertising.h"
#include "logging.h"
#include <esp_gap_ble_api.h>
#include <string.h>

// https://web.archive.org/web/20240318204212/https://developer.apple.com/accessories/Accessory-Design-Guidelines.pdf §49.4
// Advertising data should contain: Flags, TX power, Local Name, Service
// The primary services should always be advertised in the advertising PDU.
// Secondary services should not be advertised.

// Initial advertising data: Flags, TX power, Service
static esp_ble_adv_data_t adv_data = {
    .set_scan_rsp = false,
    .include_name = false,
    .include_txpower = true,
    // Intervals as multiples of 1.25 milliseconds (e.g.0x000C = 15 ms)
    // May be updated later:
    .min_interval = 0x000C,
    .max_interval = 0x000C,
    .appearance = 0x00,
    .manufacturer_len = 0,
    .p_manufacturer_data = NULL,
    .service_data_len = 0,
    .p_service_data = NULL,
    // Service advertisement will be set later:
    .service_uuid_len = 0,
    .p_service_uuid = NULL,
    .flag = ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT,
};

// Scan response: Name
static esp_ble_adv_data_t adv_data_rsp = {
    .set_scan_rsp = true,
    .include_name = true,
    .include_txpower = false,
    // Zero means not included
    .min_interval = 0x0000,
    .max_interval = 0x0000,
    .appearance = 0x00,
    .manufacturer_len = 0,
    .p_manufacturer_data = NULL,
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = 0,
    .p_service_uuid = NULL,
    .flag = ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT,
};

// https://web.archive.org/web/20240318204212/https://developer.apple.com/accessories/Accessory-Design-Guidelines.pdf §49.5
// The accessory should first use the recommended advertising interval of 20 ms for at least 30 seconds.
static esp_ble_adv_params_t adv_params = {
    .adv_int_min = 0x20, // 20 ms
    .adv_int_max = 0x20,
    .adv_type = ADV_TYPE_IND,
    .own_addr_type = BLE_ADDR_TYPE_RPA_PUBLIC,
    .channel_map = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

uint8_t adv_config_done = 0;
const uint8_t adv_config_flag = 1 << 0;
const uint8_t scan_rsp_config_flag = 1 << 1;

void advertising_set_service_uuid(const uint8_t uuid[], uint16_t length) {
    ESP_LOGI("MIDIBLE", "advertising_set_service_uuid");
    adv_data.p_service_uuid = (uint8_t *)uuid;
    adv_data.service_uuid_len = length;
}

void advertising_set_connection_interval(uint16_t itvl_min, uint16_t itvl_max) {
    adv_data.min_interval = itvl_min;
    adv_data.max_interval = itvl_max;
}

void advertising_get_connection_interval(uint16_t *itvl_min,
                                         uint16_t *itvl_max) {
    *itvl_min = adv_data.min_interval;
    *itvl_max = adv_data.max_interval;
}

bool advertising_config(void) {
    ESP_LOGI("MIDIBLE", "advertising_config");
    esp_err_t ret = esp_ble_gap_config_adv_data(&adv_data);
    if (ret) {
        ESP_LOGE("MIDIBLE", "config adv data failed, error code = %x", ret);
        return false;
    }
    adv_config_done |= adv_config_flag;
    ret = esp_ble_gap_config_adv_data(&adv_data_rsp);
    if (ret) {
        ESP_LOGE("MIDIBLE", "config adv rsp data failed, error code = %x", ret);
        return false;
    }
    adv_config_done |= scan_rsp_config_flag;
    return true;
}

bool advertising_handle_config_complete_event(esp_ble_gap_cb_param_t *param) {
    if (param->adv_data_cmpl.status != ESP_BT_STATUS_SUCCESS) {
        ESP_LOGE("MIDIBLE", "esp_ble_gap_config_adv_data failed: %d",
                 param->adv_data_cmpl.status);
        return false;
    }
    // If this completes the config, start advertising (could be before or
    // after the response config)
    adv_config_done &= (~adv_config_flag);
    if (adv_config_done == 0) {
        advertising_start();
    }
    return true;
}

bool advertising_handle_config_response_complete_event(
    esp_ble_gap_cb_param_t *param) {
    if (param->scan_rsp_data_cmpl.status != ESP_BT_STATUS_SUCCESS) {
        ESP_LOGE("MIDIBLE", "esp_ble_gap_config_adv_data response failed: %d",
                 param->scan_rsp_data_cmpl.status);
        return false;
    }
    // If this completes the config, start advertising (could be before or
    // after the advertising config)
    adv_config_done &= (~scan_rsp_config_flag);
    if (adv_config_done == 0) {
        advertising_start();
    }
    return true;
}

void advertising_start(void) { esp_ble_gap_start_advertising(&adv_params); }

#endif
#endif