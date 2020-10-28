#pragma once

/**
 * @file
 * 
 * Advertising the MIDI service for Bluetooth Low Energy.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <esp_gap_ble_api.h>

/// Set the UUID of the service to be advertised.
/// @note   UUID should persist, captures pointer, doesn't copy data.
void advertising_set_service_uuid(const uint8_t uuid[], uint16_t length);
/// Configure the advertising data, register with the Bluetooth driver. Will 
/// eventually trigger the callbacks to start advertising.
bool advertising_config(void);
/// Callback that indicates that the configuration of the advertising data was 
/// complete. Will start advertising if the configuration of the response data
/// was complete as well.
bool advertising_handle_config_complete_event(esp_ble_gap_cb_param_t *param);
/// Callback that indicates that the configuration of the advertising response 
/// data was complete. Will start advertising if the configuration of the
/// advertising data was complete as well.
bool advertising_handle_config_response_complete_event(
    esp_ble_gap_cb_param_t *param);

#ifdef __cplusplus
}
#endif