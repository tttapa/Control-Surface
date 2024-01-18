#pragma once

#include "ble.h"

#define CS_CHECK_ESP(x)                                                        \
    do {                                                                       \
        if (auto ret = x; ret != ESP_OK) {                                     \
            ESP_LOGE("CS-BLEMIDI", "Failed to call '%s': %s (%d)", #x,         \
                     esp_err_to_name(ret), static_cast<int>(ret));             \
            return false;                                                      \
        }                                                                      \
    } while (0)
#define CS_CHECK_ZERO(x)                                                       \
    do {                                                                       \
        if (auto ret = x; ret != 0) {                                          \
            ESP_LOGE("CS-BLEMIDI", "Failed to call '%s': (%d)", #x,            \
                     static_cast<int>(ret));                                   \
            return false;                                                      \
        }                                                                      \
    } while (0)
#define CS_CHECK_ZERO_V(x)                                                     \
    do {                                                                       \
        if (auto ret = x; ret != 0) {                                          \
            ESP_LOGE("CS-BLEMIDI", "Failed to call '%s': (%d)", #x,            \
                     static_cast<int>(ret));                                   \
            return;                                                            \
        }                                                                      \
    } while (0)
