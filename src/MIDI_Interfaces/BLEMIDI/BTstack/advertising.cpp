#if defined(ARDUINO_RASPBERRY_PI_PICO_W) && ENABLE_BLE

#include <btstack.h>

#include "advertising.hpp"

#include <cstring>

namespace cs::midi_ble_btstack {

namespace {

uint8_t adv_data[] {
    // Flags general discoverable
    0x02, BLUETOOTH_DATA_TYPE_FLAGS, 0x06,
    // Connection interval range
    0x05, BLUETOOTH_DATA_TYPE_SLAVE_CONNECTION_INTERVAL_RANGE, 0x0c, 0x00, 0x0c,
    0x00,
    // Service UUID
    0x11, BLUETOOTH_DATA_TYPE_COMPLETE_LIST_OF_128_BIT_SERVICE_CLASS_UUIDS,
    0x00, 0xc7, 0xc4, 0x4e, 0xe3, 0x6c, 0x51, 0xa7, 0x33, 0x4b, 0xe8, 0xed,
    0x5a, 0x0e, 0xb8, 0x03};
static_assert(sizeof(adv_data) <= LE_ADVERTISING_DATA_SIZE);
uint8_t adv_rsp_data[LE_ADVERTISING_DATA_SIZE] {
    // Name header
    0x15, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME,
    // Name value
    'C', 'o', 'n', 't', 'r', 'o', 'l', ' ', 'S', 'u', 'r', 'f', 'a', 'c', 'e',
    ' ', 'M', 'I', 'D', 'I'};
uint8_t adv_rsp_data_len() { return adv_rsp_data[0] + 1; }

void set_adv_connection_interval(uint16_t min_itvl, uint16_t max_itvl) {
    uint8_t *slave_itvl_range = adv_data + 5;
    slave_itvl_range[0] = (min_itvl >> 0) & 0xFF;
    slave_itvl_range[1] = (min_itvl >> 8) & 0xFF;
    slave_itvl_range[2] = (max_itvl >> 0) & 0xFF;
    slave_itvl_range[3] = (max_itvl >> 8) & 0xFF;
}

void set_adv_name(const char *name) {
    auto len = std::min(std::strlen(name), sizeof(adv_rsp_data) - 2);
    uint8_t *adv_name_len = adv_rsp_data;
    uint8_t *adv_name = adv_rsp_data + 2;
    std::memcpy(adv_name, name, len);
    *adv_name_len = static_cast<uint8_t>(len + 1);
}

} // namespace

void le_midi_setup_adv(const BLESettings &ble_settings) {
    set_adv_name(ble_settings.device_name);
    set_adv_connection_interval(ble_settings.connection_interval.minimum,
                                ble_settings.connection_interval.maximum);
    uint16_t adv_int_min = 0x0020; // 20 ms (multiple of 0.625ms)
    uint16_t adv_int_max = 0x0040; // 40 ms (multiple of 0.625ms)
    uint8_t adv_type = 0;
    bd_addr_t null_addr {};
    uint8_t channel_map = 0x07;   // All channels
    uint8_t filter_policy = 0x00; // Allow any
    gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0,
                                  null_addr, channel_map, filter_policy);
    gap_advertisements_set_data(sizeof(adv_data), adv_data);
    gap_scan_response_set_data(adv_rsp_data_len(), adv_rsp_data);
    gap_advertisements_enable(1);
}

} // namespace cs::midi_ble_btstack

#endif