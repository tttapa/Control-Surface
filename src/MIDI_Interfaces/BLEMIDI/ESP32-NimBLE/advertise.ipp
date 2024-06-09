#ifdef ESP32
#include <sdkconfig.h>
#if CONFIG_BT_BLE_ENABLED

#include "ble.h"

#if CS_MIDI_BLE_ESP_IDF_NIMBLE
#include <host/ble_gap.h>
#include <host/ble_hs_adv.h>
#include <services/gap/ble_svc_gap.h>
#else
#include <nimble/nimble/host/include/host/ble_gap.h>
#include <nimble/nimble/host/include/host/ble_hs_adv.h>
#include <nimble/nimble/host/services/gap/include/services/gap/ble_svc_gap.h>
#endif

#include "ble-macro-fix.h"

#include "callbacks.h"
#include "gatt.h"
#include "util.hpp"

namespace cs::midi_ble_nimble {

inline uint8_t slave_itvl_range[4] {0xFF, 0xFF, 0xFF, 0xFF};

/// Begin advertising, including the MIDI service UUID and the device name.
/// Attaches the @ref cs_midi_ble_gap_callback.
inline void advertise(uint8_t addr_type) {
    // Set the advertisement data included in our advertisements:
    //  - Flags (indicates advertisement type and other general info).
    //  - Advertising tx power.
    //  - 128-bit MIDI service UUID
    struct ble_hs_adv_fields fields {};
    // The scan response includes the following data:
    //  - Device name
    struct ble_hs_adv_fields rsp_fields {};

    //  - Discoverability in forthcoming advertisement (general)
    //  - BLE-only (BR/EDR unsupported).
    fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
    // Indicate that the TX power level field should be included; have the
    // stack fill this value automatically.
    fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;
    fields.tx_pwr_lvl_is_present = 1;
    // Advertise the MIDI service ID
    fields.uuids128 = &midi_ble_service_uuid;
    fields.num_uuids128 = 1;
    fields.uuids128_is_complete = 1;
    // Specify the desired connection interval
    fields.slave_itvl_range = slave_itvl_range;

    // Respond with the device name (it may be too long to fit in the
    // first advertisement packet, since it already contains a 16-byte UUID).
    const char *name = ble_svc_gap_device_name();
    rsp_fields.name = reinterpret_cast<const uint8_t *>(name);
    rsp_fields.name_len = strlen(name);
    rsp_fields.name_is_complete = 1;

    // Pass the data to the BLE stack
    CS_CHECK_ZERO_V(ble_gap_adv_set_fields(&fields));
    CS_CHECK_ZERO_V(ble_gap_adv_rsp_set_fields(&rsp_fields));
    // Begin advertising
    struct ble_gap_adv_params adv_params {};
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
    ESP_LOGI("CS-BLEMIDI", "Start advertising (address type=%d)", addr_type);
    CS_CHECK_ZERO_V(ble_gap_adv_start(addr_type, NULL, BLE_HS_FOREVER,
                                      &adv_params, cs_midi_ble_gap_callback,
                                      NULL));
}

inline void set_advertise_connection_interval(uint16_t min_itvl,
                                              uint16_t max_itvl) {
    slave_itvl_range[0] = (min_itvl >> 0) & 0xFF;
    slave_itvl_range[1] = (min_itvl >> 8) & 0xFF;
    slave_itvl_range[2] = (max_itvl >> 0) & 0xFF;
    slave_itvl_range[3] = (max_itvl >> 8) & 0xFF;
}

} // namespace cs::midi_ble_nimble

#endif // CONFIG_BT_BLE_ENABLED
#endif // ESP32