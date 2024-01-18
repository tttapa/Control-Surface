#pragma once

#ifdef __cplusplus
#include <AH/Arduino-Wrapper.h>
#endif

#if __has_include(<host/ble_gap.h>)
#define CS_MIDI_BLE_ESP_IDF_NIMBLE 1
#else
#include <nimconfig.h>
#define CS_MIDI_BLE_ESP_IDF_NIMBLE 0
#endif
