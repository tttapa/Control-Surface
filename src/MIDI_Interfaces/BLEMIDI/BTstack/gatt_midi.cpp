#if defined(ARDUINO_RASPBERRY_PI_PICO_W) && ENABLE_BLE

#define BTSTACK_FILE__ "gatt_midi.cpp"

#include <atomic>
#include <cassert>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <btstack.h>

#include "../BLEAPI.hpp"
#include "advertising.hpp"
#include "gatt_midi.h"
#include "hci_event_names.hpp"

#include <AH/Arduino-Wrapper.h>
#include <AH/Debug/Debug.hpp>

namespace cs::midi_ble_btstack {

namespace {

constexpr uint16_t midi_char_value_handle =
    ATT_CHARACTERISTIC_7772E5DB_3868_4112_A1A9_F2669D106BF3_01_VALUE_HANDLE;
constexpr uint16_t midi_cccd_handle =
    ATT_CHARACTERISTIC_7772E5DB_3868_4112_A1A9_F2669D106BF3_01_CLIENT_CONFIGURATION_HANDLE;

MIDIBLEInstance *instance = nullptr;
BLESettings settings;
btstack_packet_callback_registration_t hci_event_callback_registration;
btstack_packet_callback_registration_t sm_event_callback_registration;

// callback/event functions

// HCI_SUBEVENT_LE_CONNECTION_COMPLETE
void connection_handler(uint8_t *packet, [[maybe_unused]] uint16_t size) {
    if (!instance)
        return;
    if (hci_subevent_le_connection_complete_get_status(packet) != 0)
        return;
    uint16_t conn_handle =
        hci_subevent_le_connection_complete_get_connection_handle(packet);
    // Request bonding
    if (settings.initiate_security)
        sm_request_pairing(conn_handle);
    // Update the connection parameters
    uint16_t conn_latency = 0;
    uint16_t supervision_timeout = 400;
    gap_request_connection_parameter_update(
        conn_handle, settings.connection_interval.minimum,
        settings.connection_interval.maximum, conn_latency,
        supervision_timeout);
    instance->handleConnect(BLEConnectionHandle {conn_handle});
}
// HCI_SUBEVENT_LE_CONNECTION_UPDATE_COMPLETE
void connection_update_handler([[maybe_unused]] uint8_t *packet,
                               [[maybe_unused]] uint16_t size) {
    DEBUGREF( // clang-format off
        "Connection update: status="
        << hci_subevent_le_connection_update_complete_get_status(packet)
        << ", connection interval="
        << hci_subevent_le_connection_update_complete_get_conn_interval(packet)
        << ", connection latency="
        << hci_subevent_le_connection_update_complete_get_conn_latency(packet)
        << ", supervision timeout="
        << hci_subevent_le_connection_update_complete_get_supervision_timeout(packet));
              // clang-format on
}
// HCI_SUBEVENT_LE_REMOTE_CONNECTION_PARAMETER_REQUEST
void connection_param_req_handler([[maybe_unused]] uint8_t *packet,
                                  [[maybe_unused]] uint16_t size) {
    DEBUGREF( // clang-format off
        "Connection parameter request: interval min="
        << hci_subevent_le_remote_connection_parameter_request_get_interval_min(packet)
        << ", interval max="
        << hci_subevent_le_remote_connection_parameter_request_get_interval_max(packet)
        << ", latency="
        << hci_subevent_le_remote_connection_parameter_request_get_latency(packet)
        << ", timeout="
        << hci_subevent_le_remote_connection_parameter_request_get_timeout(packet));
              // clang-format on
}
// HCI_EVENT_LE_META
void le_packet_handler(uint8_t *packet, uint16_t size) {
    uint8_t type = hci_event_le_meta_get_subevent_code(packet);
    DEBUGREF("LE event: " << le_event_names[type] << " (0x" << hex << type
                          << dec << ")");
    switch (type) {
        case HCI_SUBEVENT_LE_CONNECTION_COMPLETE:
            connection_handler(packet, size);
            break;
        case HCI_SUBEVENT_LE_CONNECTION_UPDATE_COMPLETE:
            connection_update_handler(packet, size);
            break;
        case HCI_SUBEVENT_LE_REMOTE_CONNECTION_PARAMETER_REQUEST:
            connection_param_req_handler(packet, size);
            break;
        default: break;
    }
}
// HCI_EVENT_LE_META
void gattservice_handler(uint8_t *packet, [[maybe_unused]] uint16_t size) {
    [[maybe_unused]] uint8_t type =
        hci_event_gattservice_meta_get_subevent_code(packet);
    DEBUGREF("GATT service event: " << gattservice_event_names[type] << " (0x"
                                    << hex << type << dec << ")");
}
// HCI_EVENT_DISCONNECTION_COMPLETE
void disconnect_handler(uint8_t *packet, [[maybe_unused]] uint16_t size) {
    assert(instance);
    uint16_t conn_handle =
        hci_event_disconnection_complete_get_connection_handle(packet);
    instance->handleDisconnect(BLEConnectionHandle {conn_handle});
}
// ATT_EVENT_MTU_EXCHANGE_COMPLETE
void mtu_exchange_complete_handler(uint8_t *packet,
                                   [[maybe_unused]] uint16_t size) {
    assert(instance);
    uint16_t conn_handle = att_event_mtu_exchange_complete_get_handle(packet);
    uint16_t mtu = att_event_mtu_exchange_complete_get_MTU(packet);
    DEBUGREF("mtu=" << mtu);
    instance->handleMTU(BLEConnectionHandle {conn_handle}, mtu);
}
// GATT_EVENT_MTU
void gatt_event_mtu_handler(uint8_t *packet, [[maybe_unused]] uint16_t size) {
    assert(instance);
    uint16_t conn_handle = gatt_event_mtu_get_handle(packet);
    uint16_t mtu = gatt_event_mtu_get_MTU(packet);
    instance->handleMTU(BLEConnectionHandle {conn_handle}, mtu);
}
// BTSTACK_EVENT_STATE
void btstack_event_state_handler(uint8_t *packet,
                                 [[maybe_unused]] uint16_t size) {
    if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING)
        return;
    bd_addr_t local_addr;
    gap_local_bd_addr(local_addr);
    DEBUGREF("BTstack up and running on " << bd_addr_to_str(local_addr));
}

// Handles all HCI event packets.
void packet_handler(uint8_t packet_type, [[maybe_unused]] uint16_t channel,
                    uint8_t *packet, uint16_t size) {
    if (packet_type != HCI_EVENT_PACKET)
        return;
    auto type = hci_event_packet_get_type(packet);
    DEBUGREF("HCI event: " << hci_event_names[type] << " (0x" << hex << type
                           << dec << ")");
    switch (type) {
        case HCI_EVENT_LE_META: le_packet_handler(packet, size); break;
        case HCI_EVENT_GATTSERVICE_META:
            gattservice_handler(packet, size);
            break;
        case HCI_EVENT_DISCONNECTION_COMPLETE:
            disconnect_handler(packet, size);
            break;
        case SM_EVENT_JUST_WORKS_REQUEST:
            sm_just_works_confirm(
                sm_event_just_works_request_get_handle(packet));
            break;
        case ATT_EVENT_MTU_EXCHANGE_COMPLETE:
            mtu_exchange_complete_handler(packet, size);
            break;
        // TODO: what's the difference with the previous one?
        case GATT_EVENT_MTU: gatt_event_mtu_handler(packet, size); break;
        case BTSTACK_EVENT_STATE:
            btstack_event_state_handler(packet, size);
            break;
        default: break;
    }
}

// ATT Client Read Callback for Dynamic Data
// - if buffer == NULL, don't copy data, just return size of value
// - if buffer != NULL, copy data and return number bytes copied
uint16_t att_read_callback([[maybe_unused]] hci_con_handle_t connection_handle,
                           uint16_t att_handle,
                           [[maybe_unused]] uint16_t offset,
                           [[maybe_unused]] uint8_t *buffer,
                           [[maybe_unused]] uint16_t buffer_size) {
    if (att_handle == midi_char_value_handle)
        return 0; // MIDI always responds with no data
    return 0;
}

int midi_cccd_write(hci_con_handle_t conn_handle, uint8_t *buffer,
                    [[maybe_unused]] uint16_t buffer_size) {
    assert(instance);
    bool notify = (little_endian_read_16(buffer, 0) &
                   GATT_CLIENT_CHARACTERISTICS_CONFIGURATION_NOTIFICATION) != 0;
    instance->handleSubscribe(BLEConnectionHandle {conn_handle},
                              BLECharacteristicHandle {midi_char_value_handle},
                              notify);
    return 0;
}

int midi_value_write(hci_con_handle_t conn_handle, uint8_t *buffer,
                     uint16_t buffer_size) {
    assert(instance);
    BLEDataView data {buffer, buffer_size};
    auto data_gen = [data {data}]() mutable { return std::exchange(data, {}); };
    instance->handleData(
        BLEConnectionHandle {conn_handle},
        BLEDataGenerator {compat::in_place, std::move(data_gen)},
        BLEDataLifetime::ConsumeImmediately);
    return 0;
}

int att_write_callback(hci_con_handle_t conn_handle, uint16_t att_handle,
                       [[maybe_unused]] uint16_t transaction_mode,
                       uint16_t offset, uint8_t *buffer, uint16_t buffer_size) {
    DEBUGREF("ATT write: handle=" << att_handle << ", offset=" << offset
                                  << ", size=" << buffer_size);
    // Only support regular writes (no prepared/long writes)
    if (transaction_mode != ATT_TRANSACTION_MODE_NONE)
        return ATT_ERROR_REQUEST_NOT_SUPPORTED;
    // Only support writes without offset
    if (offset != 0)
        return ATT_ERROR_INVALID_OFFSET;
    // Client configuration update
    if (att_handle == midi_cccd_handle)
        return midi_cccd_write(conn_handle, buffer, buffer_size);
    // MIDI data received
    else if (att_handle == midi_char_value_handle)
        return midi_value_write(conn_handle, buffer, buffer_size);
    return 0;
}

void le_midi_setup(const BLESettings &ble_settings) {
    l2cap_init();
    // setup SM: no input, no output
    sm_init();
    sm_set_io_capabilities(IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
    sm_set_authentication_requirements(SM_AUTHREQ_SECURE_CONNECTION |
                                       SM_AUTHREQ_BONDING);
    // setup ATT server
    att_server_init(profile_data, att_read_callback, att_write_callback);
    // setup advertisements
    le_midi_setup_adv(ble_settings);
    // register for HCI events
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);
    // register for ATT events
    att_server_register_packet_handler(packet_handler);
    // register for SM events
    sm_event_callback_registration.callback = &packet_handler;
    sm_add_event_handler(&sm_event_callback_registration);
}

template <class F>
btstack_context_callback_registration_t create_context_callback(F &f) {
    btstack_context_callback_registration_t ret {};
    ret.callback = +[](void *context) { (*static_cast<F *>(context))(); };
    ret.context = &f;
    return ret;
}

} // namespace

bool init(MIDIBLEInstance &instance, BLESettings settings) {
    cs::midi_ble_btstack::instance = &instance;
    cs::midi_ble_btstack::settings = settings;
    le_midi_setup(settings);
    hci_power_control(HCI_POWER_ON);
    // btstack_run_loop_execute(); // not necessary in background mode
    return true;
}

void notify(BLEConnectionHandle conn_handle,
            BLECharacteristicHandle char_handle, BLEDataView data) {
    // DEBUGREF("[" << data.length << "] "
    //              << (AH::HexDump {data.data, data.length}));
    [[maybe_unused]] const auto t0 = micros();
    // Don't bother sending empty packets
    if (!data)
        return;
    // Flag to know when the can-send-now callback is done
    volatile std::atomic_bool notify_done {false};
    // The following is executed in the BTstack can-send-now callback, so it is
    // synchronized with the BTstack code
    auto send = [&] {
        DEBUGREF("notify " << micros() - t0);
        att_server_notify(conn_handle.conn, char_handle.characteristic,
                          data.data, data.length);
        DEBUGREF("notify done " << micros() - t0);
        notify_done.store(true, std::memory_order_release);
    };
    auto send_ctx = create_context_callback(send);
    // The following is executed in the async_context, so it is synchronized
    // with the BTstack code
    auto run = [&] {
        // Request can-send-now callback to be fired later
        DEBUGREF("req send " << micros() - t0);
        auto ret = att_server_request_to_send_notification(&send_ctx,
                                                           conn_handle.conn);
        assert(ret == ERROR_CODE_SUCCESS);
    };
    auto run_ctx = create_context_callback(run);
    // Schedule the run callback on the BTstack thread, which will then schedule
    // the send callback as soon as sending data is possible.
    DEBUGREF("req main thread " << micros() - t0);
    btstack_run_loop_execute_on_main_thread(&run_ctx);
    // Wait for the can-send-now callback to clear the flag
    while (!notify_done.load(std::memory_order_acquire)) tight_loop_contents();
    DEBUGREF("all done " << micros() - t0);
}

} // namespace cs::midi_ble_btstack

#endif