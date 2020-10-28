#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <esp_gatts_api.h>

void ble2902_handle_write(esp_gatt_if_t gatts_if,
                          esp_ble_gatts_cb_param_t *param);
bool ble2902_notifications_enabled(uint16_t handle);
bool ble2902_indications_enabled(uint16_t handle);
uint16_t ble2902_get_value(uint16_t handle);

#ifdef __cplusplus
}
#endif
