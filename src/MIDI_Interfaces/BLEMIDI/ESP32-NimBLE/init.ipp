#ifdef ESP32
#include <sdkconfig.h>
#if CONFIG_BT_BLE_ENABLED

#include "ble.h"

#if CS_MIDI_BLE_ESP_IDF_NIMBLE
#include <nimble/nimble_port.h>
#else
#include <nimble/esp_port/esp-hci/include/esp_nimble_hci.h>
#include <nimble/porting/nimble/include/nimble/nimble_port.h>
#endif
#ifdef ESP_PLATFORM
#include <esp_bt.h>
#include <nvs_flash.h>
#endif

#include "ble-macro-fix.h"
#include "util.hpp"

namespace cs::midi_ble_nimble {

inline bool init_hardware() {
#ifdef ESP_PLATFORM
#ifdef CONFIG_ENABLE_ARDUINO_DEPENDS
    // make sure the linker includes esp32-hal-bt.c so Arduino init doesn't release BLE memory.
    btStarted();
#endif
    auto nvs_flash_init_rc = nvs_flash_init();
    if (nvs_flash_init_rc == ESP_ERR_NVS_NO_FREE_PAGES ||
        nvs_flash_init_rc == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        nvs_flash_init_rc = nvs_flash_init();
    }
    ESP_ERROR_CHECK(nvs_flash_init_rc);

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
#if defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32S3)
    bt_cfg.bluetooth_mode = ESP_BT_MODE_BLE;
#else
    bt_cfg.mode = ESP_BT_MODE_BLE;
#endif
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BLE));
    ESP_ERROR_CHECK(esp_nimble_hci_init());
#endif

#if CS_MIDI_BLE_ESP_IDF_NIMBLE
    CS_CHECK_ESP(nimble_port_init());
#else
    nimble_port_init();
#endif
    return true;
}

} // namespace cs::midi_ble_nimble

#endif // CONFIG_BT_BLE_ENABLED
#endif // ESP32