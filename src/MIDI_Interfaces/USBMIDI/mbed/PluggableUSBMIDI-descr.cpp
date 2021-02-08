// Based on https://github.com/ARMmbed/mbed-os/blob/eff0d4c8b93c5a165af9f2e5e42c321a7f83eed9/drivers/usb/source/USBMIDI.cpp#L261

/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef ARDUINO_ARCH_MBED

#include "PluggableUSBMIDI.hpp"

#include <cstring> // memcpy

BEGIN_CS_NAMESPACE

const uint8_t *PluggableUSBMIDI::string_iinterface_desc() {
    // clang-format off
    static const uint8_t string_iinterface_descriptor[] {
        0x0c,                                  // bLength
        STRING_DESCRIPTOR,                     // bDescriptorType 0x03
        'A', 0, 'u', 0, 'd', 0, 'i', 0, 'o', 0 // bString iInterface - Audio
    };
    // clang-format on
    return string_iinterface_descriptor;
}

const uint8_t *PluggableUSBMIDI::configuration_desc(uint8_t index) {
    if (index != 0) {
        return nullptr;
    }

    const uint8_t wmaxpkt_L = (PacketSize >> 0) & 0xFF;
    const uint8_t wmaxpkt_M = (PacketSize >> 8) & 0xFF;

    // clang-format off
    uint8_t config_descriptor_temp[] {
        // configuration descriptor
        0x09, 0x02, 0x65, 0x00, 0x02, 0x01, 0x00, 0xc0, 0x50,

        // The Audio Interface Collection
        0x09, 0x04, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, // Standard AC Interface Descriptor
        0x09, 0x24, 0x01, 0x00, 0x01, 0x09, 0x00, 0x01, 0x01, // Class-specific AC Interface Descriptor
        0x09, 0x04, 0x01, 0x00, 0x02, 0x01, 0x03, 0x00, 0x00, // MIDIStreaming Interface Descriptors
        0x07, 0x24, 0x01, 0x00, 0x01, 0x41, 0x00,             // Class-Specific MS Interface Header Descriptor

        // MIDI IN JACKS
        0x06, 0x24, 0x02, 0x01, 0x01, 0x00,
        0x06, 0x24, 0x02, 0x02, 0x02, 0x00,

        // MIDI OUT JACKS
        0x09, 0x24, 0x03, 0x01, 0x03, 0x01, 0x02, 0x01, 0x00,
        0x09, 0x24, 0x03, 0x02, 0x06, 0x01, 0x01, 0x01, 0x00,

        // OUT endpoint - Standard MS Bulk Data Endpoint Descriptor
        0x09,         // bLength
        0x05,         // bDescriptorType
        bulk_out_ep,  // bEndpointAddress
        0x02,         // bmAttributes
        wmaxpkt_L,    // wMaxPacketSize (LSB)
        wmaxpkt_M,    // wMaxPacketSize (MSB)
        0x00,         // bInterval (milliseconds)
        0x00,         // bRefresh
        0x00,         // bSynchAddress

        0x05, 0x25, 0x01, 0x01, 0x01,

        // IN endpoint - Standard MS Bulk Data Endpoint Descriptor
        0x09,        // bLength
        0x05,        // bDescriptorType
        bulk_in_ep,  // bEndpointAddress
        0x02,        // bmAttributes
        wmaxpkt_L,   // wMaxPacketSize (LSB)
        wmaxpkt_M,   // wMaxPacketSize (MSB)
        0x00,        // bInterval (milliseconds)
        0x00,        // bRefresh
        0x00,        // bSynchAddress

        0x05, 0x25, 0x01, 0x01, 0x03,
    };
    // clang-format on
    static_assert(sizeof(config_descriptor_temp) == sizeof(config_descriptor),
                  "Descriptor size error");
    memcpy(config_descriptor, config_descriptor_temp,
           sizeof(config_descriptor_temp));
    return config_descriptor;
}

END_CS_NAMESPACE

#endif