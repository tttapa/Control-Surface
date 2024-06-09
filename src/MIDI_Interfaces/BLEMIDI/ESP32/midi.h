#pragma once

/** 
 * @file
 * 
 * Public MIDI over Bluetooth Low Energy API.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Send a MIDI BLE notification to the client.
bool midi_notify(uint16_t conn_handle, uint16_t char_handle,
                 const uint8_t *data, size_t len);

/// Set the name of the BLE device. Must be set before calling @ref midi_init().
void set_midi_ble_name(const char *name);
/// Configure whether the Arduino should initiate the bonding/secure connection.
void set_security_initiate_encryption(bool security_initiate_encryption_);

/// Initialize the Bluetooth stack and register the MIDI BLE application with
/// the Bluedroid driver.
bool midi_init();
/// Cleanup the MIDI BLE application and de-initialize the Bluetooth stack.
/// @todo   Implement (currently throws an assertion error).
bool midi_deinit();

#ifdef __cplusplus
}
#endif