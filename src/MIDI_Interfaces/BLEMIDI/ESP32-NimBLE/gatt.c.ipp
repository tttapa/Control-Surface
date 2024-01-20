#ifdef ESP32
#include <sdkconfig.h>
#if CONFIG_BT_BLE_ENABLED

#include "ble.h"

#if CS_MIDI_BLE_ESP_IDF_NIMBLE
#include <host/ble_uuid.h>
#else
#include <nimble/nimble/host/include/host/ble_uuid.h>
#endif

#include "callbacks.h"

#ifdef __cplusplus

// This should really be done in C, but we have to compile only if the user
// includes these files explicitly, so we rely on some GCC C++ extensions here.

inline const ble_uuid128_t midi_ble_service_uuid {
    .u {.type = BLE_UUID_TYPE_128},
    .value {0x00, 0xc7, 0xc4, 0x4e, 0xe3, 0x6c, //
            0x51, 0xa7,                         //
            0x33, 0x4b,                         //
            0xe8, 0xed,                         //
            0x5a, 0x0e, 0xb8, 0x03},
};
inline const ble_uuid128_t midi_ble_characteristic_uuid {
    .u {.type = BLE_UUID_TYPE_128},
    .value {0xf3, 0x6b, 0x10, 0x9d, 0x66, 0xf2, //
            0xa9, 0xa1,                         //
            0x12, 0x41,                         //
            0x68, 0x38,                         //
            0xdb, 0xe5, 0x72, 0x77},
};

inline const struct ble_gatt_chr_def midi_ble_characteristic[] = {
    {.uuid = &midi_ble_characteristic_uuid.u,
     .access_cb = cs_midi_ble_characteristic_callback,
     .arg = nullptr,
     .descriptors = nullptr,
     .flags = BLE_GATT_CHR_F_READ |         // BLE_GATT_CHR_F_READ_ENC |
              BLE_GATT_CHR_F_WRITE_NO_RSP | // BLE_GATT_CHR_F_WRITE_ENC |
              BLE_GATT_CHR_F_NOTIFY,
     .min_key_size = 0,
     .val_handle = nullptr},
    {}, // sentinel
};

inline const struct ble_gatt_svc_def midi_ble_service[] = {
    {.type = BLE_GATT_SVC_TYPE_PRIMARY,
     .uuid = &midi_ble_service_uuid.u,
     .includes = nullptr,
     .characteristics = midi_ble_characteristic},
    {}, // sentinel
};

#else

const ble_uuid128_t midi_ble_service_uuid =
    BLE_UUID128_INIT(0x00, 0xc7, 0xc4, 0x4e, 0xe3, 0x6c, //
                     0x51, 0xa7,                         //
                     0x33, 0x4b,                         //
                     0xe8, 0xed,                         //
                     0x5a, 0x0e, 0xb8, 0x03);
const ble_uuid128_t midi_ble_characteristic_uuid =
    BLE_UUID128_INIT(0xf3, 0x6b, 0x10, 0x9d, 0x66, 0xf2, //
                     0xa9, 0xa1,                         //
                     0x12, 0x41,                         //
                     0x68, 0x38,                         //
                     0xdb, 0xe5, 0x72, 0x77);

static const struct ble_gatt_chr_def midi_ble_characteristic[] = {
    {.uuid = &midi_ble_characteristic_uuid.u,
     .access_cb = cs_midi_ble_characteristic_callback,
     .flags = BLE_GATT_CHR_F_READ |         // BLE_GATT_CHR_F_READ_ENC |
              BLE_GATT_CHR_F_WRITE_NO_RSP | // BLE_GATT_CHR_F_WRITE_ENC |
              BLE_GATT_CHR_F_NOTIFY},
    {0}, // sentinel
};

static const struct ble_gatt_svc_def midi_ble_service[] = {
    {.type = BLE_GATT_SVC_TYPE_PRIMARY,
     .uuid = &midi_ble_service_uuid.u,
     .characteristics = midi_ble_characteristic},
    {0}, // sentinel
};

#endif

inline const struct ble_gatt_svc_def *midi_ble_get_service(void) {
    return midi_ble_service;
}

#endif // CONFIG_BT_BLE_ENABLED
#endif // ESP32
