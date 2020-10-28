#ifdef ESP32

/**
 * @file
 * 
 * Event handler of Bluetooth Low Energy, such as read, write, app registration,
 * connect/disconnect, MTU negotiations ... dispatching them to the right
 * functions for the MIDI application.
 */

#include "ble2902.h"
#include "logging.h"
#include "midi-private.h"

void midi_handle_gatts_event(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                             esp_ble_gatts_cb_param_t *param) {
    // If this event is not specific to an interface, we're interested
    // If this event is for our interface, we're interested as well.
    bool for_us = gatts_if == ESP_GATT_IF_NONE || //
                  gatts_if == midi_get_gatts_if();
    if (!for_us)
        return;

    switch (event) {
        case ESP_GATTS_REG_EVT:
            midi_handle_register_app_event(gatts_if, param);
            break;

        case ESP_GATTS_READ_EVT:
            if (param->read.handle == midi_get_characteristic_handle())
                midi_handle_read_event(gatts_if, param);
            break;

        case ESP_GATTS_WRITE_EVT:
            // If it's a short write to our 2902 descriptor
            if (param->write.is_prep == false &&
                midi_get_descriptor_handle() == param->write.handle) {
                ble2902_handle_write(gatts_if, param);
            }
            // If it's a write (short or long) to our MIDI characteristic
            if (midi_get_characteristic_handle() == param->write.handle)
                midi_handle_write_event(gatts_if, param);
            break;
        case ESP_GATTS_EXEC_WRITE_EVT:
            midi_handle_write_exec_event(gatts_if, param);
            break;

        case ESP_GATTS_MTU_EVT: midi_handle_mtu_event(gatts_if, param); break;

        case ESP_GATTS_CONF_EVT: break;
        case ESP_GATTS_UNREG_EVT: break;
        case ESP_GATTS_CREATE_EVT: break;
        case ESP_GATTS_ADD_INCL_SRVC_EVT: break;
        case ESP_GATTS_ADD_CHAR_EVT: break;
        case ESP_GATTS_ADD_CHAR_DESCR_EVT: break;
        case ESP_GATTS_DELETE_EVT: break;
        case ESP_GATTS_START_EVT: break;
        case ESP_GATTS_STOP_EVT: break;
        case ESP_GATTS_CONNECT_EVT:
            midi_handle_connect_event(gatts_if, param);
            break;
        case ESP_GATTS_DISCONNECT_EVT:
            midi_handle_disconnect_event(gatts_if, param);
            break;
        case ESP_GATTS_OPEN_EVT: break;
        case ESP_GATTS_CANCEL_OPEN_EVT: break;
        case ESP_GATTS_CLOSE_EVT: break;
        case ESP_GATTS_LISTEN_EVT: break;
        case ESP_GATTS_CONGEST_EVT: break;
        case ESP_GATTS_RESPONSE_EVT: break;
        case ESP_GATTS_CREAT_ATTR_TAB_EVT:
            if (param->add_attr_tab.status != ESP_GATT_OK) {
                ESP_LOGE("MIDIBLE",
                         "create attribute table failed, error code=0x%x",
                         param->add_attr_tab.status);
                break;
            }
            midi_handle_create_attribute_table_event(gatts_if, param);
            break;

        case ESP_GATTS_SET_ATTR_VAL_EVT: break;
        case ESP_GATTS_SEND_SERVICE_CHANGE_EVT: break;
        default: break;
    }
}

#endif