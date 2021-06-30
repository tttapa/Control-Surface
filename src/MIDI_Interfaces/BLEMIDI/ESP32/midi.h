#pragma once

/** 
 * @file
 * 
 * Public MIDI over Bluetooth Low Energy API.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Type for the MTU negotiation callback.
/// @warning    Used in C code, use C calling convention (use `extern "C"` to
///             declare this function in C++ code).
typedef void (*midi_mtu_callback_t)(uint16_t);
/// Set the callback that is to be called when the MTU negotiation with the BLE
/// client is finished.
void midi_set_mtu_callback(midi_mtu_callback_t cb);
/// Get the current MTU (maximum transmission unit) of the link with the BLE
/// client.
uint16_t midi_get_mtu(void);

/// Type for the BLE MIDI write callback.
/// @warning    Used in C code, use C calling convention (use `extern "C"` to
///             declare this function in C++ code).
typedef void (*midi_write_callback_t)(const uint8_t *, size_t);
/// Set the callback that is to be called when the client writes (sends) a MIDI
/// packet.
void midi_set_write_callback(midi_write_callback_t cb);

/// Send a MIDI BLE notification to the client.
bool midi_notify(const uint8_t *data, size_t len);

/// Initialize the Bluetooth stack and register the MIDI BLE application with 
/// the Bluedroid driver.
bool midi_init();
/// Cleanup the MIDI BLE application and de-initialize the Bluetooth stack.
/// @todo   Implement (currently throws an assertion error).
bool midi_deinit();

#ifdef __cplusplus
}
#endif