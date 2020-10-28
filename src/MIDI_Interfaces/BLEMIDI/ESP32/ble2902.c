#ifdef ESP32

#include "ble2902.h"
#include "logging.h"

#include <string.h> // memcpy

void ble2902_handle_write(esp_gatt_if_t gatts_if,
                          esp_ble_gatts_cb_param_t *param) {
    // The actual writing of data and sending a response is already handled
    // by Bluedroid.
    uint16_t handle = param->write.handle;
    if (ble2902_get_value(handle) == 0x0001) {
        ESP_LOGI("MIDIBLE", "notify enable");
    } else if (ble2902_get_value(handle) == 0x0002) {
        ESP_LOGI("MIDIBLE", "indicate enable");
    } else if (ble2902_get_value(handle) == 0x0003) {
        ESP_LOGI("MIDIBLE", "notify & indicate enable");
    } else if (ble2902_get_value(handle) == 0x0000) {
        ESP_LOGI("MIDIBLE", "notify/indicate disable ");
    } else {
        ESP_LOGE("MIDIBLE", "Unknown descriptor value %04x",
                 ble2902_get_value(handle));
    }
}

uint16_t ble2902_get_value(uint16_t handle) {
    const uint8_t *data;
    uint16_t length;
    esp_ble_gatts_get_attr_value(handle, &length, &data);
    if (length != 2) {
        ESP_LOGE("MIDIBLE", "Unexpected descriptor value length (%d)", length);
        return 0;
    }
    uint16_t ret;
    memcpy(&ret, data, length);
    return ret;
}

bool ble2902_notifications_enabled(uint16_t handle) {
    return ble2902_get_value(handle) & 0x0001;
}

bool ble2902_indications_enabled(uint16_t handle) {
    return ble2902_get_value(handle) & 0x0002;
}

#endif