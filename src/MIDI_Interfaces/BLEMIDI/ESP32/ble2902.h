#pragma once

/**
 * @file
 * 
 * Handling the Client Characteristic Configuration Descriptor (UUID 0x2902) for
 * MIDI over Bluetooth Low Energy.
 * Determines whether notifications are enabled or not.
 * 
 * @todo    According to the spec, this value should persist for bonded devices.
 *          This is currently not implemented, and would require storing a 
 *          database of bonded devices with their CCCD values in NVS. Maybe 
 *          there's a way to let Bluedroid handle this, since it already keeps
 *          track of bonded devices?
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <esp_gatts_api.h>

/// Callback when the client writes to the descriptor. Currently only prints
/// debug information or error messages.
void ble2902_handle_write(esp_gatt_if_t gatts_if,
                          esp_ble_gatts_cb_param_t *param);
/// Check if notifications are enabled by the client.
/// @param  handle
///         The handle of the CCC descriptor.
bool ble2902_notifications_enabled(uint16_t handle);
/// Check if indications are enabled by the client.
/// @param  handle
///         The handle of the CCC descriptor.
bool ble2902_indications_enabled(uint16_t handle);
/// Get the value of the descriptor.
/// @param  handle
///         The handle of the CCC descriptor.
uint16_t ble2902_get_value(uint16_t handle);

#ifdef __cplusplus
}
#endif
