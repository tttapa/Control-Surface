#ifdef ESP32
#include <sdkconfig.h>
#if CONFIG_BT_BLE_ENABLED

#include "ble.h"

#if CS_MIDI_BLE_ESP_IDF_NIMBLE
#include <host/ble_gatt.h>
#include <host/ble_hs.h>
#include <nimble/nimble_port.h>
#include <nimble/nimble_port_freertos.h>
#include <services/gap/ble_svc_gap.h>
#include <services/gatt/ble_svc_gatt.h>
#else
#include <nimble/nimble/host/include/host/ble_gatt.h>
#include <nimble/nimble/host/include/host/ble_hs.h>
#include <nimble/nimble/host/services/gap/include/services/gap/ble_svc_gap.h>
#include <nimble/nimble/host/services/gatt/include/services/gatt/ble_svc_gatt.h>
#include <nimble/porting/nimble/include/nimble/nimble_port.h>
#include <nimble/porting/npl/freertos/include/nimble/nimble_port_freertos.h>
#endif

#include "ble-macro-fix.h"

#include "app.hpp"
#include "callbacks.h"
#include "events.hpp"
#include "gatt.h"
#include "init.hpp"
#include "util.hpp"

inline cs::midi_ble_nimble::MIDIBLEState cs_midi_ble_state;

extern "C" void ble_store_config_init(void);

namespace cs::midi_ble_nimble {

inline bool init(MIDIBLEInstance &instance, BLESettings ble_settings) {
    // Configure the hardware to support BLE using NimBLE.
    if (!init_hardware())
        return false;

    // Initialize the NimBLE host configuration.
    ble_hs_cfg.reset_cb = cs_midi_ble_on_reset;
    ble_hs_cfg.sync_cb = cs_midi_ble_on_sync;
    ble_hs_cfg.gatts_register_cb = cs_midi_ble_service_register_callback;
    ble_hs_cfg.store_status_cb = ble_store_util_status_rr;
    // XXX Need to have template for store
    ble_store_config_init();

    // No input or output capabilities.
    ble_hs_cfg.sm_io_cap = BLE_SM_IO_CAP_NO_IO;
    ble_hs_cfg.sm_bonding = 1;
    // Enable the appropriate bit masks to make sure the keys that are needed
    // are exchanged
    ble_hs_cfg.sm_our_key_dist |= BLE_SM_PAIR_KEY_DIST_ENC;
    ble_hs_cfg.sm_their_key_dist |= BLE_SM_PAIR_KEY_DIST_ENC;
    ble_hs_cfg.sm_mitm = 1;
    ble_hs_cfg.sm_sc = 1;
    // Stores the IRK
    ble_hs_cfg.sm_our_key_dist |= BLE_SM_PAIR_KEY_DIST_ID;
    ble_hs_cfg.sm_their_key_dist |= BLE_SM_PAIR_KEY_DIST_ID;

    CS_CHECK_ZERO(ble_gatts_reset());
    ble_svc_gap_init();
    ble_svc_gatt_init();

    // Set the maximum transmission unit for better throughput
    CS_CHECK_ZERO(ble_att_set_preferred_mtu(BLE_ATT_MTU_MAX));

    // Initialize the MIDI service and characteristic
    cs_midi_ble_state.instance = &instance;
    cs_midi_ble_state.settings = ble_settings;
    cs::midi_ble_nimble::state = &cs_midi_ble_state;
    const auto *gatt_server_services = midi_ble_get_service();
    CS_CHECK_ZERO(ble_gatts_count_cfg(gatt_server_services));
    CS_CHECK_ZERO(ble_gatts_add_svcs(gatt_server_services));

    // Set the default device name
    CS_CHECK_ZERO(ble_svc_gap_device_name_set(ble_settings.device_name));
    set_advertise_connection_interval(ble_settings.connection_interval.minimum,
                                      ble_settings.connection_interval.maximum);

    // Start the FreeRTOS task that runs the NimBLE stack
    nimble_port_freertos_init([](void *) {
        ESP_LOGI("CS-BLEMIDI", "BLE Host Task Started");
        // This function will return only when nimble_port_stop() is executed
        nimble_port_run();
        ESP_LOGI("CS-BLEMIDI", "BLE Host Task Ended");
        nimble_port_freertos_deinit();
    });
    return true;
}

inline bool notify(BLEConnectionHandle conn_handle,
                   BLECharacteristicHandle char_handle, BLEDataView data) {
    auto om = ble_hs_mbuf_from_flat(data.data, data.length);
    CS_CHECK_ZERO(ble_gattc_notify_custom(conn_handle.conn,
                                          char_handle.characteristic, om));
    return true;
}

} // namespace cs::midi_ble_nimble

#endif // CONFIG_BT_BLE_ENABLED
#endif // ESP32