#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*midi_mtu_callback_t)(uint16_t);
void midi_set_mtu_callback(midi_mtu_callback_t cb);
uint16_t midi_get_mtu(void);

typedef void (*midi_write_callback_t)(const uint8_t *, size_t);
void midi_set_write_callback(midi_write_callback_t cb);

bool midi_notify(const uint8_t *data, size_t len);

bool midi_init();
bool midi_deinit();

#ifdef __cplusplus
}
#endif