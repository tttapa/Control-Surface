#pragma once

#ifndef CS_USB_MIDI_NOT_SUPPORTED

#include <AH/Containers/Array.hpp>
#include <AH/STL/cstdint>
#include <Settings/NamespaceSettings.hpp>

#ifdef ARDUINO_ARCH_ESP32
#include <esp_arduino_version.h>
#include <sdkconfig.h>
#endif

BEGIN_CS_NAMESPACE

inline AH::Array<uint8_t, 4> u32_to_bytes(uint32_t u) {
    return {{
        uint8_t((u >> 0) & 0xFF),
        uint8_t((u >> 8) & 0xFF),
        uint8_t((u >> 16) & 0xFF),
        uint8_t((u >> 24) & 0xFF),
    }};
}

inline uint32_t bytes_to_u32(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) {
    return (uint32_t(b0) << 0) |  //
           (uint32_t(b1) << 8) |  //
           (uint32_t(b2) << 16) | //
           (uint32_t(b3) << 24);  //
}

inline uint32_t bytes_to_u32(AH::Array<uint8_t, 4> b) {
    return (uint32_t(b.data[0]) << 0) |  //
           (uint32_t(b.data[1]) << 8) |  //
           (uint32_t(b.data[2]) << 16) | //
           (uint32_t(b.data[3]) << 24);  //
}

END_CS_NAMESPACE

#ifdef ARDUINO

#if defined(TEENSYDUINO)

#if defined(__AVR__) /* Teensy 2.x */
#include "USBMIDI_Teensy2.hpp"
BEGIN_CS_NAMESPACE
using USBDeviceMIDIBackend = Teensy2_USBDeviceMIDIBackend;
END_CS_NAMESPACE

#elif defined(__MK20DX128__)  /* Teensy 3.0 */                                 \
    || defined(__MK20DX256__) /* Teensy 3.1/3.2 */                             \
    || defined(__MK64FX512__) /* Teensy 3.5 */                                 \
    || defined(__MK66FX1M0__) /* Teensy 3.6 */
#include "USBMIDI_Teensy3.hpp"
BEGIN_CS_NAMESPACE
using USBDeviceMIDIBackend = Teensy3_USBDeviceMIDIBackend;
END_CS_NAMESPACE

#elif defined(__IMXRT1062__) || defined(__IMXRT1052__) /* Teensy 4.0 */
#include "USBMIDI_Teensy4.hpp"
BEGIN_CS_NAMESPACE
using USBDeviceMIDIBackend = Teensy4_USBDeviceMIDIBackend;
END_CS_NAMESPACE

#elif defined(__MKL26Z64__) /* Teensy LC */
#include "USBMIDI_TeensyLC.hpp"
BEGIN_CS_NAMESPACE
using USBDeviceMIDIBackend = TeensyLC_USBDeviceMIDIBackend;
END_CS_NAMESPACE

#else
#warning "Unknown Teensy board, please open an issue on GitHub" \
         "https://github.com/tttapa/Arduino-Helpers"
// Fall back to the MIDIUSB library and hope for the best
#include "USBMIDI_MIDIUSB.hpp"
BEGIN_CS_NAMESPACE
using USBDeviceMIDIBackend = MIDIUSB_USBDeviceMIDIBackend;
END_CS_NAMESPACE
#endif

#elif defined(ARDUINO_ARCH_ESP32) && (ESP_ARDUINO_VERSION_MAJOR >= 3) &&       \
    (CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3)

#ifndef ARDUINO_USB_MODE
#error "ESP32-S2/S3 expects ARDUINO_USB_MODE to be set"
#endif
#if ARDUINO_USB_MODE != 1
#include "USBMIDI_ESP32.hpp"
BEGIN_CS_NAMESPACE
using USBDeviceMIDIBackend = ESP32_USBDeviceMIDIBackend;
END_CS_NAMESPACE
#else
#define CS_USB_MIDI_NOT_SUPPORTED 1
#pragma message(                                                               \
    "ESP32-S2/S3: USB MIDI not enabled. Set the Tools > USB Type setting to \"USB-OTG\" to enable it.")
#endif

#elif defined(ARDUINO_ARCH_MBED)

#include "USBMIDI_Arduino_mbed.hpp"
BEGIN_CS_NAMESPACE
using USBDeviceMIDIBackend = Arduino_mbed_USBDeviceMIDIBackend;
END_CS_NAMESPACE

#elif defined(ARDUINO_ARCH_RP2040) && defined(USE_TINYUSB)

#include "USBMIDI_Adafruit_TinyUSB.hpp"
BEGIN_CS_NAMESPACE
using USBDeviceMIDIBackend = Adafruit_TinyUSB_USBDeviceMIDIBackend;
END_CS_NAMESPACE

#else

#include <AH/Arduino-Wrapper.h>
#ifdef USBCON
#include "USBMIDI_MIDIUSB.hpp"
BEGIN_CS_NAMESPACE
using USBDeviceMIDIBackend = MIDIUSB_USBDeviceMIDIBackend;
END_CS_NAMESPACE
#else
#define CS_USB_MIDI_NOT_SUPPORTED 1
#endif

#endif

#else

#include "USBMIDI_Mock.hpp"

#endif

#endif // CS_USB_MIDI_NOT_SUPPORTED
