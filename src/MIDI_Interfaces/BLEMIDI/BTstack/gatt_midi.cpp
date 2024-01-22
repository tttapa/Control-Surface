#if defined(ARDUINO_RASPBERRY_PI_PICO_W) && ENABLE_BLE

#define BTSTACK_FILE__ "gatt_midi.cpp"

#include <atomic>
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

#include <AH/Arduino-Wrapper.h>

namespace cs::midi_ble_btstack {

namespace {

constexpr uint16_t midi_char_value_handle =
    ATT_CHARACTERISTIC_7772E5DB_3868_4112_A1A9_F2669D106BF3_01_VALUE_HANDLE;

MIDIBLEInstance *instance = nullptr;

struct {
    BLEDataView data {};
    uint16_t conn_handle = 0xFFFF;
    uint16_t char_handle = 0xFFFF;
} to_notify {};
volatile sig_atomic_t notify_done = false;

btstack_packet_callback_registration_t hci_event_callback_registration;

void packet_handler(uint8_t packet_type, uint16_t channel,
                           uint8_t *packet, uint16_t size);
uint16_t att_read_callback(hci_con_handle_t con_handle,
                                  uint16_t att_handle, uint16_t offset,
                                  uint8_t *buffer, uint16_t buffer_size);
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

void le_midi_setup(void) {

    l2cap_init();

    Serial.println("a");

    // setup SM: Display only
    sm_init();
    Serial.println("b");

    // setup ATT server
    att_server_init(profile_data, att_read_callback, att_write_callback);
    Serial.println("c");

    // setup advertisements
    uint16_t adv_int_min = 0x0030;
    uint16_t adv_int_max = 0x0030;
    uint8_t adv_type = 0;
    bd_addr_t null_addr;
    memset(null_addr, 0, 6);
    gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0,
                                  null_addr, 0x07, 0x00);
    gap_advertisements_set_data(sizeof(adv_data), (uint8_t *)adv_data);
    gap_advertisements_enable(1);
    Serial.println("d");

    // register for HCI events
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);
    Serial.println("e");

    // register for ATT event
    att_server_register_packet_handler(packet_handler);
    Serial.println("f");
}



void packet_handler(uint8_t packet_type, uint16_t channel,
                           uint8_t *packet, uint16_t size) {
    UNUSED(channel);
    UNUSED(size);

    if (packet_type != HCI_EVENT_PACKET)
        return;

    auto type = hci_event_packet_get_type(packet);
    printf("packet_handler: type=%d\n", (int)type);
    switch (type) {
        case HCI_EVENT_LE_META: {
            // Wait for connection complete
            switch (hci_event_le_meta_get_subevent_code(packet)) {
                case HCI_SUBEVENT_LE_CONNECTION_COMPLETE: {
                    if (hci_subevent_le_connection_complete_get_status(
                            packet) == 0) {
                        uint16_t conn_handle =
                            hci_subevent_le_connection_complete_get_connection_handle(
                                packet);
                        if (instance)
                            instance->handleConnect(
                                BLEConnectionHandle {conn_handle});
                    }
                } break;
                default: break;
            }
        } break;
        case HCI_EVENT_DISCONNECTION_COMPLETE: {
            uint16_t conn_handle =
                hci_event_disconnection_complete_get_connection_handle(packet);
            if (instance)
                instance->handleDisconnect(BLEConnectionHandle {conn_handle});
        } break;
        case ATT_EVENT_CAN_SEND_NOW: {
            if (to_notify.data) {
                att_server_notify(to_notify.conn_handle, to_notify.char_handle,
                                  to_notify.data.data, to_notify.data.length);
                std::atomic_signal_fence(std::memory_order_release);
                notify_done = true;
            }
        } break;
        default: break;
        case ATT_EVENT_MTU_EXCHANGE_COMPLETE: {
            uint16_t conn_handle =
                att_event_mtu_exchange_complete_get_handle(packet);
            uint16_t mtu = att_event_mtu_exchange_complete_get_MTU(packet);
            if (instance)
                instance->handleMTU(BLEConnectionHandle {conn_handle}, mtu);
        } break;
        // TODO: what's the difference with the previous one?
        case GATT_EVENT_MTU: {
            uint16_t conn_handle = gatt_event_mtu_get_handle(packet);
            uint16_t mtu = gatt_event_mtu_get_MTU(packet);
            if (instance)
                instance->handleMTU(BLEConnectionHandle {conn_handle}, mtu);
        } break;
        case BTSTACK_EVENT_STATE: {
            if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING)
                return;
            bd_addr_t local_addr;
            gap_local_bd_addr(local_addr);
            printf("BTstack up and running on %s.\n",
                   bd_addr_to_str(local_addr));
        } break;
    }
}

/* LISTING_END */

/*
 * @section ATT Read
 *
 * @text The ATT Server handles all reads to constant data. For dynamic data like the custom characteristic, the registered
 * att_read_callback is called. To handle long characteristics and long reads, the att_read_callback is first called
 * with buffer == NULL, to request the total value length. Then it will be called again requesting a chunk of the value.
 * See Listing attRead.
 */

/* LISTING_START(attRead): ATT Read */

// ATT Client Read Callback for Dynamic Data
// - if buffer == NULL, don't copy data, just return size of value
// - if buffer != NULL, copy data and return number bytes copied
// @param offset defines start of attribute value
uint16_t att_read_callback(hci_con_handle_t connection_handle,
                                  uint16_t att_handle, uint16_t offset,
                                  uint8_t *buffer, uint16_t buffer_size) {
    UNUSED(connection_handle);
    UNUSED(offset);
    UNUSED(buffer);
    UNUSED(buffer_size);

    if (att_handle == midi_char_value_handle) {
        return 0; // Respond with no data
    }
    return 0;
}
/* LISTING_END */

int att_write_callback(hci_con_handle_t conn_handle, uint16_t att_handle,
                              uint16_t transaction_mode, uint16_t offset,
                              uint8_t *buffer, uint16_t buffer_size) {
    UNUSED(transaction_mode);

    Serial << "att_write_callback: " << att_handle << endl;

    if (att_handle ==
        ATT_CHARACTERISTIC_7772E5DB_3868_4112_A1A9_F2669D106BF3_01_CLIENT_CONFIGURATION_HANDLE) {
        if (instance) {
            bool notify =
                (little_endian_read_16(buffer, 0) &
                 GATT_CLIENT_CHARACTERISTICS_CONFIGURATION_NOTIFICATION) != 0;
            instance->handleSubscribe(
                BLEConnectionHandle {conn_handle},
                BLECharacteristicHandle {midi_char_value_handle}, notify);
        }
    } else if (att_handle == midi_char_value_handle && offset == 0) {
        if (instance) {
            BLEDataView data {buffer, buffer_size};
            auto data_gen = [data {data}]() mutable {
                return std::exchange(data, {});
            };
            instance->handleData(
                BLEConnectionHandle {conn_handle},
                BLEDataGenerator {compat::in_place, std::move(data_gen)},
                BLEDataLifetime::ConsumeImmediately);
        }
    }
    return 0;
}
/* LISTING_END */

} // namespace

bool init(MIDIBLEInstance &instance, BLESettings settings) {
    Serial.println("A");
    Serial.println("B");
    cs::midi_ble_btstack::instance = &instance;
    Serial.println("C");
    le_midi_setup();
    Serial.println("D");
    hci_power_control(HCI_POWER_ON);
    Serial.println("E");
    // btstack_run_loop_execute();
    // Serial.println("F");
    return true;
}

void notify(BLEConnectionHandle conn_handle,
            BLECharacteristicHandle char_handle, BLEDataView data) {
    if (!data)
        return;
    Serial.println("r");
    auto run = [&] {
        to_notify.data = data;
        to_notify.conn_handle = conn_handle.conn;
        to_notify.char_handle = char_handle.characteristic;
        att_server_request_can_send_now_event(to_notify.conn_handle);
    };
    notify_done = false;
    async_context_execute_sync(
        cyw43_arch_async_context(),
        +[](void *arg) -> uint32_t {
            (*static_cast<decltype(run) *>(arg))();
            return 0;
        },
        &run);
    while (!notify_done) tight_loop_contents();
    std::atomic_signal_fence(std::memory_order_acquire);
    Serial.println("d");
}

} // namespace cs::midi_ble_btstack

#endif