#ifdef ESP32
#include <sdkconfig.h>
#if CONFIG_BT_BLE_ENABLED

#include "advertise.hpp"
#include "callbacks.h"
#include "events.hpp"
#include "gatt.h"
#include "util.hpp"

#if CS_MIDI_BLE_ESP_IDF_NIMBLE
#include <host/ble_hs_pvcy.h>
#include <host/util/util.h>
#else
#include <nimble/nimble/host/include/host/ble_hs_pvcy.h>
#include <nimble/nimble/host/util/include/host/util/util.h>
#endif

namespace cs::midi_ble_nimble {
inline MIDIBLEState *state;

namespace {

[[maybe_unused]] std::string fmt_address(const void *addr) {
    std::string str {"XX:XX:XX:XX:XX:XX"};
    auto *u8p = reinterpret_cast<const uint8_t *>(addr);
    snprintf(str.data(), str.size() + 1, "%02x:%02x:%02x:%02x:%02x:%02x",
             u8p[5], u8p[4], u8p[3], u8p[2], u8p[1], u8p[0]);
    return str;
}

void print_conn_desc(struct ble_gap_conn_desc *desc) {
    ESP_LOGI(
        "CS-BLEMIDI",
        "connection info: handle=%d\n"
        "    our_ota_addr_type=%d our_ota_addr=%s\n"
        "    our_id_addr_type=%d our_id_addr=%s\n"
        "    peer_ota_addr_type=%d peer_ota_addr=%s\n"
        "    peer_id_addr_type=%d peer_id_addr=%s\n"
        "    conn_itvl=%d conn_latency=%d supervision_timeout=%d\n"
        "    encrypted=%d authenticated=%d bonded=%d",
        desc->conn_handle, desc->our_ota_addr.type,
        fmt_address(desc->our_ota_addr.val).c_str(), desc->our_id_addr.type,
        fmt_address(desc->our_id_addr.val).c_str(), desc->peer_ota_addr.type,
        fmt_address(desc->peer_ota_addr.val).c_str(), desc->peer_id_addr.type,
        fmt_address(desc->peer_id_addr.val).c_str(), desc->conn_itvl,
        desc->conn_latency, desc->supervision_timeout,
        desc->sec_state.encrypted, desc->sec_state.authenticated,
        desc->sec_state.bonded);
}

const char *own_address_type_to_string(uint8_t own_addr_type) {
    switch (own_addr_type) {
        case BLE_OWN_ADDR_RANDOM: return "BLE_OWN_ADDR_RANDOM";
        case BLE_OWN_ADDR_RPA_RANDOM_DEFAULT:
            return "BLE_OWN_ADDR_RPA_RANDOM_DEFAULT";
        case BLE_OWN_ADDR_PUBLIC: return "BLE_OWN_ADDR_PUBLIC";
        case BLE_OWN_ADDR_RPA_PUBLIC_DEFAULT:
            return "BLE_OWN_ADDR_RPA_PUBLIC_DEFAULT";
        default: return "<invalid>";
    }
}

} // namespace

} // namespace cs::midi_ble_nimble

/// Called when the host and controller become synced (i.e. after successful
/// startup).
inline void cs_midi_ble_on_sync() {
    ESP_LOGD("CS-BLEMIDI", "sync");
    // Enable privacy if supported
#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
    int privacy = 1;
    // Use a resolvable private address
    CS_CHECK_ZERO_V(ble_hs_pvcy_rpa_config(NIMBLE_HOST_ENABLE_RPA));
    // Configure the device with at least one address (1=prefer random)
    CS_CHECK_ZERO_V(ble_hs_util_ensure_addr(1));
#else
    int privacy = 0;
    // Configure the device with at least one address (0=prefer public)
    CS_CHECK_ZERO_V(ble_hs_util_ensure_addr(0));
#endif
    // Determine the best address type to use
    auto &addr_type = cs::midi_ble_nimble::state->address_type;
    CS_CHECK_ZERO_V(ble_hs_id_infer_auto(privacy, &addr_type));
    uint8_t id_addr_type = BLE_ADDR_PUBLIC;
    if (addr_type & (BLE_OWN_ADDR_RANDOM | BLE_OWN_ADDR_RPA_RANDOM_DEFAULT))
        id_addr_type = BLE_ADDR_RANDOM;
    ESP_LOGI("CS-BLEMIDI", "Using address type %s (%d)",
             cs::midi_ble_nimble::own_address_type_to_string(addr_type),
             addr_type);
    uint8_t addr_val[6] = {0};
    CS_CHECK_ZERO_V(ble_hs_id_copy_addr(id_addr_type, addr_val, NULL));
    ESP_LOGD("CS-BLEMIDI", "address=%s",
             cs::midi_ble_nimble::fmt_address(addr_val).c_str());
    cs::midi_ble_nimble::advertise(addr_type);
}

/// Called when the stack is reset.
inline void cs_midi_ble_on_reset(int reason) {
    ESP_LOGE("CS-BLEMIDI", "Resetting state; reason=%d", reason);
}

/// Called when any GATT characteristic (or descriptor) is accessed.
inline int
cs_midi_ble_characteristic_callback(uint16_t conn_handle, uint16_t attr_handle,
                                    struct ble_gatt_access_ctxt *ctxt, void *) {
    ESP_LOGD("CS-BLEMIDI", "gatt callback %d", ctxt->op);
    switch (ctxt->op) {
        // READ: BLE MIDI should respond with no payload
        case BLE_GATT_ACCESS_OP_READ_CHR: {
            ESP_LOGD("CS-BLEMIDI", "gatt read");
            static uint8_t dummy = 0;
            auto rc = os_mbuf_append(ctxt->om, &dummy, 0);
            return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
        } break;

        // Write: pass the received MIDI data on to the driver
        case BLE_GATT_ACCESS_OP_WRITE_CHR: {
            ESP_LOGD("CS-BLEMIDI", "gatt write");
            auto data_gen = [om {ctxt->om}]() mutable {
                if (!om)
                    return cs::BLEDataView {};
                cs::BLEDataView data {om->om_data, om->om_len};
                om = om->om_next.sle_next;
                return data;
            };
            if (auto *inst = cs::midi_ble_nimble::state->instance)
                inst->handleData(cs::BLEConnectionHandle {conn_handle},
                                 cs::BLEDataGenerator {cs::compat::in_place,
                                                       std::move(data_gen)},
                                 cs::BLEDataLifetime::ConsumeImmediately);
            return 0;
        } break;

        default: return 0;
    }
}

/// Called during GATT initialization, for each service, characteristic and
/// descriptor that is registered.
inline void
cs_midi_ble_service_register_callback(struct ble_gatt_register_ctxt *ctxt,
                                      void *) {
    ESP_LOGI("CS-BLEMIDI", "service event %d", ctxt->op);
    [[maybe_unused]] char buf[BLE_UUID_STR_LEN] {};

    switch (ctxt->op) {
        case BLE_GATT_REGISTER_OP_SVC:
            ESP_LOGI("CS-BLEMIDI", "registering service %s with handle=%d",
                     ble_uuid_to_str(ctxt->svc.svc_def->uuid, buf),
                     ctxt->svc.handle);
            break;

        case BLE_GATT_REGISTER_OP_CHR:
            ESP_LOGI("CS-BLEMIDI",
                     "registering characteristic %s with "
                     "def_handle=%d val_handle=%d",
                     ble_uuid_to_str(ctxt->chr.chr_def->uuid, buf),
                     ctxt->chr.def_handle, ctxt->chr.val_handle);
            // Save the handle to the MIDI characteristic
            if (ble_uuid_cmp(ctxt->chr.chr_def->uuid,
                             &midi_ble_characteristic_uuid.u) == 0) {
                ESP_LOGI("CS-BLEMIDI", "MIDI char: %d", ctxt->chr.val_handle);
                cs::midi_ble_nimble::state->midi_characteristic_handle =
                    ctxt->chr.val_handle;
            }
            break;

        case BLE_GATT_REGISTER_OP_DSC:
            ESP_LOGI("CS-BLEMIDI", "registering descriptor %s with handle=%d",
                     ble_uuid_to_str(ctxt->dsc.dsc_def->uuid, buf),
                     ctxt->dsc.handle);
            break;

        default: ESP_LOGI("CS-BLEMIDI", "Unknown op %d", ctxt->op); break;
    }
}

namespace {
inline void update_connection_params(uint16_t conn_handle,
                                     const cs::BLESettings &settings) {
    ble_gap_upd_params params {};
    params.latency = 0;
    params.itvl_max = settings.connection_interval.maximum;
    params.itvl_min = settings.connection_interval.minimum;
    params.supervision_timeout = 400;
    // Connection event length
    params.min_ce_len = BLE_GAP_INITIAL_CONN_MIN_CE_LEN;
    params.max_ce_len = BLE_GAP_INITIAL_CONN_MAX_CE_LEN;
    if (auto rc = ble_gap_update_params(conn_handle, &params); rc != 0) {
        ESP_LOGE("CS-BLEMIDI", "failed to update params; rc=%d", rc);
    }
}
} // namespace

/// Called for GAP events like (dis)connection, MTU updates, etc.
inline int cs_midi_ble_gap_callback(struct ble_gap_event *event, void *) {
    ESP_LOGI("CS-BLEMIDI", "gap event %d", +event->type);
    switch (event->type) {
        // A new connection was established or a connection attempt failed
        case BLE_GAP_EVENT_CONNECT: {
            ESP_LOGI("CS-BLEMIDI", "connection %s; status=%d",
                     event->connect.status == 0 ? "established" : "failed",
                     event->connect.status);
            if (event->connect.status == 0) {
                auto conn_handle = event->connect.conn_handle;
                const auto &settings = cs::midi_ble_nimble::state->settings;
                struct ble_gap_conn_desc desc;
                auto rc = ble_gap_conn_find(conn_handle, &desc);
                assert(rc == 0);
                cs::midi_ble_nimble::print_conn_desc(&desc);
                // TODO: is this the correct place?
                // Without this, Android does not initiate bonding or SC, but
                // with it, Windows starts a connect/disconnect loop on the
                // second connection ...
                if (settings.initiate_security) {
                    if (auto rc = ble_gap_security_initiate(conn_handle);
                        rc != 0) {
                        ESP_LOGE("CS-BLEMIDI",
                                 "failed to initiate secure connection; rc=%d",
                                 rc);
                        return ble_gap_terminate(conn_handle,
                                                 BLE_ERR_REM_USER_CONN_TERM);
                    }
                }
                // Update the connection parameters
                update_connection_params(conn_handle, settings);
                if (auto *inst = cs::midi_ble_nimble::state->instance)
                    inst->handleConnect(cs::BLEConnectionHandle {conn_handle});
            } else {
                // Connection failed; resume advertising
                cs::midi_ble_nimble::advertise(
                    cs::midi_ble_nimble::state->address_type);
            }
        } break;

        // Disconnected
        case BLE_GAP_EVENT_DISCONNECT: {
            ESP_LOGI("CS-BLEMIDI", "disconnect; reason=%d",
                     event->disconnect.reason);
            cs::midi_ble_nimble::print_conn_desc(&event->disconnect.conn);

            if (auto *inst = cs::midi_ble_nimble::state->instance)
                inst->handleDisconnect(cs::BLEConnectionHandle {
                    event->disconnect.conn.conn_handle});
            // Connection terminated; resume advertising
            cs::midi_ble_nimble::advertise(
                cs::midi_ble_nimble::state->address_type);
        } break;

        // Central has updated the connection parameters
        case BLE_GAP_EVENT_CONN_UPDATE: {
            ESP_LOGI("CS-BLEMIDI", "connection updated; status=%d ",
                     event->conn_update.status);
            struct ble_gap_conn_desc desc;
            auto rc = ble_gap_conn_find(event->conn_update.conn_handle, &desc);
            assert(rc == 0);
            cs::midi_ble_nimble::print_conn_desc(&desc);
        } break;

        // Advertising done (e.g. after reaching the specified timeout)
        case BLE_GAP_EVENT_ADV_COMPLETE: {
            ESP_LOGI("CS-BLEMIDI", "adv complete");
            cs::midi_ble_nimble::advertise(
                cs::midi_ble_nimble::state->address_type);
        } break;

        // Encryption has been enabled or disabled for this connection
        case BLE_GAP_EVENT_ENC_CHANGE: {
            ESP_LOGI("CS-BLEMIDI", "encryption change event; status=%d ",
                     event->enc_change.status);
            struct ble_gap_conn_desc desc;
            auto rc = ble_gap_conn_find(event->enc_change.conn_handle, &desc);
            assert(rc == 0);
            cs::midi_ble_nimble::print_conn_desc(&desc);
        } break;

        // Subscription (e.g. when a CCCD is updated)
        case BLE_GAP_EVENT_SUBSCRIBE: {
            ESP_LOGI("CS-BLEMIDI",
                     "subscribe event; cur_notify=%d val_handle=%d",
                     event->subscribe.cur_notify, event->subscribe.attr_handle);
            if (event->subscribe.attr_handle ==
                cs::midi_ble_nimble::state->midi_characteristic_handle)
                if (auto *inst = cs::midi_ble_nimble::state->instance)
                    inst->handleSubscribe(
                        cs::BLEConnectionHandle {event->subscribe.conn_handle},
                        cs::BLECharacteristicHandle {
                            event->subscribe.attr_handle},
                        event->subscribe.cur_notify);
        } break;

        // MTU updated (used to update the packet/buffer size)
        case BLE_GAP_EVENT_MTU: {
            ESP_LOGI("CS-BLEMIDI", "mtu update event; conn_handle=%d mtu=%d",
                     event->mtu.conn_handle, event->mtu.value);
            if (auto *inst = cs::midi_ble_nimble::state->instance)
                inst->handleMTU(
                    cs::BLEConnectionHandle {event->mtu.conn_handle},
                    event->mtu.value);
        } break;

        // Repeat pairing
        case BLE_GAP_EVENT_REPEAT_PAIRING: {
            ESP_LOGI("CS-BLEMIDI", "repeat pairing event; conn_handle=%d",
                     event->repeat_pairing.conn_handle);
            // We already have a bond with the peer, but it is attempting to
            // establish a new secure link.  This app sacrifices security for
            // convenience: just throw away the old bond and accept the new link.

            // Delete the old bond.
            struct ble_gap_conn_desc desc;
            auto rc =
                ble_gap_conn_find(event->repeat_pairing.conn_handle, &desc);
            assert(rc == 0);
            ble_store_util_delete_peer(&desc.peer_id_addr);

            // Return BLE_GAP_REPEAT_PAIRING_RETRY to indicate that the host should
            // continue with the pairing operation.
            return BLE_GAP_REPEAT_PAIRING_RETRY;
        }
    }

    return 0;
}

#endif // CONFIG_BT_BLE_ENABLED
#endif // ESP32