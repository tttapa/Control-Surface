#if defined(ARDUINO_RASPBERRY_PI_PICO_W) && ENABLE_BLE

#define BTSTACK_FILE__ "gatt_midi.cpp"

#include <atomic>
#include <cassert>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <btstack.h>
#include <btstack_event.h>
#include <pico/async_context.h>
#include <pico/cyw43_arch.h>

#include "../BLEAPI.hpp"
#include "gatt_midi.h"
#include "hci_event_names.hpp"

#include <AH/Arduino-Wrapper.h>

namespace cs::midi_ble_btstack {

namespace {

constexpr uint16_t midi_char_value_handle =
    ATT_CHARACTERISTIC_7772E5DB_3868_4112_A1A9_F2669D106BF3_01_VALUE_HANDLE;
constexpr uint16_t midi_cccd_handle =
    ATT_CHARACTERISTIC_7772E5DB_3868_4112_A1A9_F2669D106BF3_01_CLIENT_CONFIGURATION_HANDLE;

MIDIBLEInstance *instance = nullptr;

struct {
    BLEDataView data {};
    uint16_t conn_handle = 0xFFFF;
    uint16_t char_handle = 0xFFFF;
} to_notify {};
volatile sig_atomic_t notify_done = false;

btstack_packet_callback_registration_t hci_event_callback_registration;

void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet,
                    uint16_t size);
uint16_t att_read_callback(hci_con_handle_t con_handle, uint16_t att_handle,
                           uint16_t offset, uint8_t *buffer,
                           uint16_t buffer_size);
int att_write_callback(hci_con_handle_t con_handle, uint16_t att_handle,
                       uint16_t transaction_mode, uint16_t offset,
                       uint8_t *buffer, uint16_t buffer_size);

const uint8_t adv_data[] {
    // Flags general discoverable
    0x02, BLUETOOTH_DATA_TYPE_FLAGS, 0x06,
    // Name
    0x05, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'M', 'I', 'D', 'I',
    // Service UUID
    0x11, BLUETOOTH_DATA_TYPE_COMPLETE_LIST_OF_128_BIT_SERVICE_CLASS_UUIDS,
    0x00, 0xc7, 0xc4, 0x4e, 0xe3, 0x6c, 0x51, 0xa7, 0x33, 0x4b, 0xe8, 0xed,
    0x5a, 0x0e, 0xb8, 0x03};
static_assert(sizeof(adv_data) <= LE_ADVERTISING_DATA_SIZE);

void le_midi_setup_adv() {
    uint16_t adv_int_min = 0x0030;
    uint16_t adv_int_max = 0x0030;
    uint8_t adv_type = 0;
    bd_addr_t null_addr;
    memset(null_addr, 0, 6);
    gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0,
                                  null_addr, 0x07, 0x00);
    gap_advertisements_set_data(sizeof(adv_data), (uint8_t *)adv_data);
    gap_advertisements_enable(1);
}

void le_midi_setup() {
    l2cap_init();
    // setup SM: no input, no output
    sm_init();
    // setup ATT server
    att_server_init(profile_data, att_read_callback, att_write_callback);
    // setup advertisements
    le_midi_setup_adv();
    // register for HCI events
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);
    // register for ATT event
    att_server_register_packet_handler(packet_handler);
}

// HCI_SUBEVENT_LE_CONNECTION_COMPLETE
void connection_handler(uint8_t *packet, uint16_t size) {
    if (!instance)
        return;
    if (hci_subevent_le_connection_complete_get_status(packet) != 0)
        return;
    uint16_t conn_handle =
        hci_subevent_le_connection_complete_get_connection_handle(packet);
    instance->handleConnect(BLEConnectionHandle {conn_handle});
}
// HCI_EVENT_LE_META
void le_packet_handler(uint8_t *packet, uint16_t size) {
    uint8_t type = hci_event_le_meta_get_subevent_code(packet);
    printf("LE event: %s (0x%02x)\n", le_event_names[type], (int)type);
    switch (type) {
        case HCI_SUBEVENT_LE_CONNECTION_COMPLETE:
            connection_handler(packet, size);
            break;
        case HCI_SUBEVENT_LE_CONNECTION_UPDATE_COMPLETE:
            printf(
                "Connection update: status=%d, connection interval=%d, "
                "connection latency=%d, supervision timeout=%d\n",
                hci_subevent_le_connection_update_complete_get_status(packet),
                hci_subevent_le_connection_update_complete_get_conn_interval(
                    packet),
                hci_subevent_le_connection_update_complete_get_conn_latency(
                    packet),
                hci_subevent_le_connection_update_complete_get_supervision_timeout(
                    packet));
            break;
        case HCI_SUBEVENT_LE_REMOTE_CONNECTION_PARAMETER_REQUEST:
            printf(
                "Connection parameter request: interval min=%d, interval "
                "max=%d, "
                "latency=%d, timeout=%d\n",
                hci_subevent_le_remote_connection_parameter_request_get_interval_min(
                    packet),
                hci_subevent_le_remote_connection_parameter_request_get_interval_max(
                    packet),
                hci_subevent_le_remote_connection_parameter_request_get_latency(
                    packet),
                hci_subevent_le_remote_connection_parameter_request_get_timeout(
                    packet));
            break;
        default: break;
    }
}
// HCI_EVENT_LE_META
void gattservice_handler(uint8_t *packet, uint16_t size) {
    uint8_t type = hci_event_gattservice_meta_get_subevent_code(packet);
    printf("GATT service event: %s (0x%02x)\n", gattservice_event_names[type],
           (int)type);
}
// HCI_EVENT_DISCONNECTION_COMPLETE
void disconnect_handler(uint8_t *packet, uint16_t size) {
    assert(instance);
    uint16_t conn_handle =
        hci_event_disconnection_complete_get_connection_handle(packet);
    instance->handleDisconnect(BLEConnectionHandle {conn_handle});
}
// ATT_EVENT_CAN_SEND_NOW
void can_send_now_handler() {
    if (!to_notify.data)
        return;
    att_server_notify(to_notify.conn_handle, to_notify.char_handle,
                      to_notify.data.data, to_notify.data.length);
    std::atomic_signal_fence(std::memory_order_release);
    notify_done = true;
}
// ATT_EVENT_MTU_EXCHANGE_COMPLETE
void mtu_exchange_complete_handler(uint8_t *packet, uint16_t size) {
    assert(instance);
    uint16_t conn_handle = att_event_mtu_exchange_complete_get_handle(packet);
    uint16_t mtu = att_event_mtu_exchange_complete_get_MTU(packet);
    printf("MTU: %d\n", (int)mtu);
    instance->handleMTU(BLEConnectionHandle {conn_handle}, mtu);
}
// GATT_EVENT_MTU
void gatt_event_mtu_handler(uint8_t *packet, uint16_t size) {
    assert(instance);
    uint16_t conn_handle = gatt_event_mtu_get_handle(packet);
    uint16_t mtu = gatt_event_mtu_get_MTU(packet);
    instance->handleMTU(BLEConnectionHandle {conn_handle}, mtu);
}
// BTSTACK_EVENT_STATE
void btstack_event_state_handler(uint8_t *packet, uint16_t size) {
    if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING)
        return;
    bd_addr_t local_addr;
    gap_local_bd_addr(local_addr);
    printf("BTstack up and running on %s.\n", bd_addr_to_str(local_addr));
}

// Handles all HCI event packets.
void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet,
                    uint16_t size) {
    if (packet_type != HCI_EVENT_PACKET)
        return;
    auto type = hci_event_packet_get_type(packet);
    printf("HCI event: %s (0x%02x)\n", hci_event_names[type], (int)type);
    switch (type) {
        case HCI_EVENT_LE_META: le_packet_handler(packet, size); break;
        case HCI_EVENT_GATTSERVICE_META:
            gattservice_handler(packet, size);
            break;
        case HCI_EVENT_DISCONNECTION_COMPLETE:
            disconnect_handler(packet, size);
            break;
        case ATT_EVENT_CAN_SEND_NOW: can_send_now_handler(); break;
        default: break;
        case ATT_EVENT_MTU_EXCHANGE_COMPLETE:
            mtu_exchange_complete_handler(packet, size);
            break;
        // TODO: what's the difference with the previous one?
        case GATT_EVENT_MTU: gatt_event_mtu_handler(packet, size); break;
        case BTSTACK_EVENT_STATE:
            btstack_event_state_handler(packet, size);
            break;
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
    Serial << "att_write_callback: " << att_handle << endl;
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

template <class F>
uint32_t cyw43_arch_async_context_execute_sync_callable(F &&f) {
    return async_context_execute_sync(
        cyw43_arch_async_context(),
        +[](void *arg) -> uint32_t {
            return (*static_cast<std::remove_reference_t<F> *>(arg))();
        },
        &f);
}

} // namespace

bool init(MIDIBLEInstance &instance, BLESettings settings) {
    cs::midi_ble_btstack::instance = &instance;
    le_midi_setup();
    hci_power_control(HCI_POWER_ON);
    // btstack_run_loop_execute(); // not necessary in background mode
    return true;
}

void notify(BLEConnectionHandle conn_handle,
            BLECharacteristicHandle char_handle, BLEDataView data) {
    // Don't bother sending empty packets
    if (!data)
        return;
    // The following is executed in the async_context, so it is synchronized
    // with the BTstack code
    auto run = [&] {
        // Store the data to send
        to_notify.data = data;
        to_notify.conn_handle = conn_handle.conn;
        to_notify.char_handle = char_handle.characteristic;
        // Request can-send-now event to be fired later
        att_server_request_can_send_now_event(to_notify.conn_handle);
        return 0;
    };
    // Flag to know when the can-send-now event is done
    notify_done = false;
    std::atomic_signal_fence(std::memory_order_release);
    // Run the code above code in the async_context (synchronously)
    cyw43_arch_async_context_execute_sync_callable(run);
    // Wait for the can-send-now event to clear the flag
    while (!notify_done) tight_loop_contents();
    std::atomic_signal_fence(std::memory_order_acquire);
}

} // namespace cs::midi_ble_btstack

#endif