#ifdef ESP32

/** 
 * @file
 * 
 * Definition of the MIDI Bluetooth Low Energy UUIDs, services, characteristics,
 * descriptors ... Handles app registration events, keeps track of the BLE 
 * handles, connection ID, application ID, GATT server interface ID ...
 */

#include "advertising.h"
#include "logging.h"
#include "midi-private.h"

#include <esp_gatt_defs.h>
#include <stddef.h> // NULL
#include <string.h> // memcpy

/// https://www.midi.org/specifications/item/bluetooth-le-midi (MMA/AMEI RP-052)
/// 03b80e5a-ede8-4b33-a751-6ce34ec4c700
static const uint8_t MIDI_SERVICE_UUID_128[16] = {
    0x00, 0xc7, 0xc4, 0x4e, 0xe3, 0x6c, //
    0x51, 0xa7,                         //
    0x33, 0x4b,                         //
    0xe8, 0xed,                         //
    0x5a, 0x0e, 0xb8, 0x03,             //
};

/// https://www.midi.org/specifications/item/bluetooth-le-midi (MMA/AMEI RP-052)
/// 7772e5db-3868-4112-a1a9-f2669d106bf3
static const uint8_t MIDI_char_uuid128[16] = {
    0xf3, 0x6b, 0x10, 0x9d, 0x66, 0xf2, //
    0xa9, 0xa1,                         //
    0x12, 0x41,                         //
    0x68, 0x38,                         //
    0xdb, 0xe5, 0x72, 0x77,             //
};

/// Spec mandates read support (respond with no payload), notify support,
/// and write support (without response).
const esp_gatt_char_prop_t MIDI_properties = ESP_GATT_CHAR_PROP_BIT_READ |
                                             ESP_GATT_CHAR_PROP_BIT_WRITE_NR |
                                             ESP_GATT_CHAR_PROP_BIT_NOTIFY;

/// Indices for the service table
enum {
    MIDI_SERVICE_INDEX,
    MIDI_CHARACTERISTIC_DECLARATION_INDEX,
    MIDI_CHARACTERISTIC_VALUE_INDEX,
    MIDI_2902_DESCRIPTOR_INDEX,
    MIDI_ATTRIBUTE_TABLE_SIZE,
};

// <?> What does this do?
// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/bluetooth/esp_gatts.html#_CPPv429esp_ble_gatts_create_attr_tabPK19esp_gatts_attr_db_t13esp_gatt_if_t7uint8_t7uint8_t
static const uint8_t MIDI_SERVICE_INSTANCE_ID = 0;

static const uint16_t primary_service_uuid = ESP_GATT_UUID_PRI_SERVICE;
static const uint16_t character_declaration_uuid = ESP_GATT_UUID_CHAR_DECLARE;
static const uint16_t character_client_config_uuid =
    ESP_GATT_UUID_CHAR_CLIENT_CONFIG;

/* Full Database Description - Used to add attributes into the database */
static const esp_gatts_attr_db_t
    midi_attribute_table[MIDI_ATTRIBUTE_TABLE_SIZE] = {
        // Service Declaration
        [MIDI_SERVICE_INDEX] =
            {
                .attr_control = {ESP_GATT_AUTO_RSP},
                {
                    .uuid_length = sizeof(primary_service_uuid),
                    .uuid_p = (uint8_t *)&primary_service_uuid,
                    .perm = ESP_GATT_PERM_READ,
                    .max_length = sizeof(MIDI_SERVICE_UUID_128),
                    .length = sizeof(MIDI_SERVICE_UUID_128),
                    .value = (uint8_t *)MIDI_SERVICE_UUID_128,
                },
            },

        // Characteristic Declaration
        [MIDI_CHARACTERISTIC_DECLARATION_INDEX] =
            {
                .attr_control = {ESP_GATT_AUTO_RSP},
                {
                    .uuid_length = sizeof(character_declaration_uuid),
                    .uuid_p = (uint8_t *)&character_declaration_uuid,
                    .perm = ESP_GATT_PERM_READ,
                    .max_length = sizeof(MIDI_properties),
                    .length = sizeof(MIDI_properties),
                    .value = (uint8_t *)&MIDI_properties,
                },
            },

        // Characteristic Value
        [MIDI_CHARACTERISTIC_VALUE_INDEX] =
            {
                .attr_control = {ESP_GATT_RSP_BY_APP},
                {
                    .uuid_length = sizeof(MIDI_char_uuid128),
                    .uuid_p = (uint8_t *)&MIDI_char_uuid128,
                    .perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                    .max_length = 0,
                    .length = 0,
                    .value = NULL,
                },
            },

        // Client Characteristic Configuration Descriptor (0x2902)
        [MIDI_2902_DESCRIPTOR_INDEX] =
            {
                .attr_control = {ESP_GATT_AUTO_RSP},
                {
                    .uuid_length = sizeof(character_client_config_uuid),
                    .uuid_p = (uint8_t *)&character_client_config_uuid,
                    .perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                    .max_length = 2,
                    .length = 0,
                    .value = NULL,
                },
            },
};

static uint16_t MIDI_handle_table[MIDI_ATTRIBUTE_TABLE_SIZE] = {};

static uint16_t midi_gatts_if = ESP_GATT_IF_NONE;
static uint16_t midi_conn_id = 0;

void midi_register_interface(esp_gatt_if_t gatts_if) {
    midi_gatts_if = gatts_if;
}

void midi_set_connection_id(uint16_t conn_id) { midi_conn_id = conn_id; }
uint16_t midi_get_connection_id(void) { return midi_conn_id; }

void midi_handle_register_app_event(esp_gatt_if_t gatts_if,
                                    esp_ble_gatts_cb_param_t *param) {
    // Set the Service UUID for advertisement
    advertising_set_service_uuid(MIDI_SERVICE_UUID_128,
                                 sizeof(MIDI_SERVICE_UUID_128));
    // Configure the advertising data and start advertising when that's done
    advertising_config();

    // Register the GATTS service table
    esp_err_t ret = esp_ble_gatts_create_attr_tab(
        midi_attribute_table, gatts_if, MIDI_ATTRIBUTE_TABLE_SIZE,
        MIDI_SERVICE_INSTANCE_ID);
    if (ret) {
        ESP_LOGE("MIDIBLE", "Create attribute table failed, error code: %d",
                 ret);
    }
}

void midi_handle_create_attribute_table_event(esp_gatt_if_t gatts_if,
                                              esp_ble_gatts_cb_param_t *param) {
    if (param->add_attr_tab.num_handle != MIDI_ATTRIBUTE_TABLE_SIZE) {
        ESP_LOGE("MIDIBLE",
                 "Create attribute table error, num_handle (%d) "
                 "doesn't equal MIDI_ATTRIBUTE_TABLE_SIZE (%d)",
                 param->add_attr_tab.num_handle, MIDI_ATTRIBUTE_TABLE_SIZE);
        return;
    }
    ESP_LOGI("MIDIBLE",
             "Created attribute table successfully, number of "
             "handles: %d\n",
             param->add_attr_tab.num_handle);
    // Save the handles to the created services, characteristics and
    // desriptors
    memcpy(MIDI_handle_table, param->add_attr_tab.handles,
           sizeof(MIDI_handle_table));
    // Start the MIDI service.
    esp_ble_gatts_start_service(midi_get_service_handle());
}

uint16_t midi_get_service_handle(void) {
    return MIDI_handle_table[MIDI_SERVICE_INDEX];
}

uint16_t midi_get_characteristic_handle(void) {
    return MIDI_handle_table[MIDI_CHARACTERISTIC_VALUE_INDEX];
}

uint16_t midi_get_descriptor_handle(void) {
    return MIDI_handle_table[MIDI_2902_DESCRIPTOR_INDEX];
}

uint16_t midi_get_app_id(void) { return 0x55; }
uint16_t midi_get_gatts_if(void) { return midi_gatts_if; }

#endif