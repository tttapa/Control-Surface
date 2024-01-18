#ifdef ESP32
#include <sdkconfig.h>
#if CONFIG_BT_BLE_ENABLED

#include "callbacks.h"
#include "events.hpp"
#include "gatt.h"
#include "util.hpp"

namespace cs::midi_ble {
void advertise(uint8_t addr_type);
MIDIBLEState *state;
} // namespace cs::midi_ble

static void print_addr(const void *addr) {
    auto *u8p = reinterpret_cast<const uint8_t *>(addr);
    ESP_LOGI("CS-BLEMIDI", "address: %02x:%02x:%02x:%02x:%02x:%02x", u8p[5],
             u8p[4], u8p[3], u8p[2], u8p[1], u8p[0]);
}

/// Called when the host and controller become synced (i.e. after successful
/// startup).
void cs_midi_ble_on_sync() {
    ESP_LOGD("CS-BLEMIDI", "sync");
    CS_CHECK_ZERO_V(
        ble_hs_id_infer_auto(0, &cs::midi_ble::state->address_type));
    uint8_t addr_val[6] = {0};
    CS_CHECK_ZERO_V(
        ble_hs_id_copy_addr(cs::midi_ble::state->address_type, addr_val, NULL));
    print_addr(addr_val);
    cs::midi_ble::advertise(cs::midi_ble::state->address_type);
}

/// Called when the stack is reset.
void cs_midi_ble_on_reset(int reason) {
    ESP_LOGE("CS-BLEMIDI", "Resetting state; reason=%d", reason);
}

/// Called when any GATT characteristic (or descriptor) is accessed.
int cs_midi_ble_characteristic_callback(uint16_t conn_handle,
                                        uint16_t attr_handle,
                                        struct ble_gatt_access_ctxt *ctxt,
                                        void *) {
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
            if (auto *inst = cs::midi_ble::state->instance)
                inst->handleData(cs::BLEConnectionHandle {conn_handle},
                                 cs::BLEDataGenerator {std::in_place, data_gen},
                                 cs::BLEDataLifetime::ConsumeImmediately);
            return 0;
        } break;

        default: return 0;
    }
}

/// Called during GATT initialization, for each service, characteristic and
/// descriptor that is registered.
void cs_midi_ble_service_register_callback(struct ble_gatt_register_ctxt *ctxt,
                                           void *) {
    ESP_LOGI("CS-BLEMIDI", "service event %d", ctxt->op);
    char buf[BLE_UUID_STR_LEN] {};

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
                cs::midi_ble::state->midi_characteristic_handle =
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

/// Called for GAP events like (dis)connection, MTU updates, etc.
int cs_midi_ble_gap_callback(struct ble_gap_event *event, void *) {
    ESP_LOGI("CS-BLEMIDI", "gap event %d", +event->type);
    switch (event->type) {
        // A new connection was established or a connection attempt failed
        case BLE_GAP_EVENT_CONNECT:
            ESP_LOGI("CS-BLEMIDI", "connection %s; status=%d",
                     event->connect.status == 0 ? "established" : "failed",
                     event->connect.status);
            if (auto rc = ble_att_set_preferred_mtu(512); rc != 0) {
                ESP_LOGE("CS-BLEMIDI", "failed to set preferred MTU; rc=%d",
                         rc);
                return rc;
            }

            if (event->connect.status == 0) {
                if (auto *inst = cs::midi_ble::state->instance)
                    inst->handleConnect(
                        cs::BLEConnectionHandle {event->connect.conn_handle});
            } else {
                // Connection failed; resume advertising
                cs::midi_ble::advertise(cs::midi_ble::state->address_type);
            }
            break;

        // Disconnected
        case BLE_GAP_EVENT_DISCONNECT:
            ESP_LOGI("CS-BLEMIDI", "disconnect; reason=%d",
                     event->disconnect.reason);

            if (auto *inst = cs::midi_ble::state->instance)
                inst->handleDisconnect(cs::BLEConnectionHandle {
                    event->disconnect.conn.conn_handle});
            // Connection terminated; resume advertising
            cs::midi_ble::advertise(cs::midi_ble::state->address_type);
            break;

        // Advertising done (e.g. after reaching the specified timeout)
        case BLE_GAP_EVENT_ADV_COMPLETE:
            ESP_LOGI("CS-BLEMIDI", "adv complete");
            cs::midi_ble::advertise(cs::midi_ble::state->address_type);
            break;

        // Subscription (e.g. when a CCCD is updated)
        case BLE_GAP_EVENT_SUBSCRIBE:
            ESP_LOGI("CS-BLEMIDI",
                     "subscribe event; cur_notify=%d val_handle=%d",
                     event->subscribe.cur_notify, event->subscribe.attr_handle);
            if (event->subscribe.attr_handle ==
                cs::midi_ble::state->midi_characteristic_handle)
                if (auto *inst = cs::midi_ble::state->instance)
                    inst->handleSubscribe(
                        cs::BLEConnectionHandle {event->subscribe.conn_handle},
                        cs::BLECharacteristicHandle {
                            event->subscribe.attr_handle},
                        event->subscribe.cur_notify);
            break;

        // MTU updated (used to update the packet/buffer size)
        case BLE_GAP_EVENT_MTU:
            ESP_LOGI("CS-BLEMIDI", "mtu update event; conn_handle=%d mtu=%d",
                     event->mtu.conn_handle, event->mtu.value);
            if (auto *inst = cs::midi_ble::state->instance)
                inst->handleMTU(
                    cs::BLEConnectionHandle {event->mtu.conn_handle},
                    event->mtu.value);
            break;
    }

    return 0;
}

#endif // CONFIG_BT_BLE_ENABLED
#endif // ESP32