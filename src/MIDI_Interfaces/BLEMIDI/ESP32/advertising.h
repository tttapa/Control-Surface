#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <esp_gap_ble_api.h>

void advertising_set_service_uuid(const uint8_t uuid[], uint16_t length);
bool advertising_config(void);
bool advertising_handle_config_complete_event(esp_ble_gap_cb_param_t *param);
bool advertising_handle_config_response_complete_event(
    esp_ble_gap_cb_param_t *param);

#ifdef __cplusplus
}
#endif