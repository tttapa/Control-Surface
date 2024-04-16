#pragma once

#ifndef CS_BLE_MIDI_NOT_SUPPORTED

#include <Settings/NamespaceSettings.hpp>

#ifdef DOXYGEN
BEGIN_CS_NAMESPACE
/// Default backend for the @ref BluetoothMIDI_Interface class.
/// @see @ref md_pages_MIDI-over-BLE
struct BLEMIDIBackend {};
END_BEGIN_CS_NAMESPACE
/// Indicates whether @ref BLEMIDIBackend and @ref BluetoothMIDI_Interface are
/// defined for this board.
#define CS_BLE_MIDI_SUPPORTED 1
/// On ESP32, changes the default MIDI over BLE backend from Bluedroid to NimBLE.
/// This macro should be defined before including any Control Surface headers.
/// Requires the [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino) library.
#define CS_USE_NIMBLE

#elif defined(ESP32)
#include <sdkconfig.h>
#if CONFIG_BT_BLE_ENABLED
// ESP32 with BLE support
#ifdef CS_USE_NIMBLE
// NimBLE backend
#include "BLEMIDI/ESP32NimBLEBackend.hpp"
BEGIN_CS_NAMESPACE
using BLEMIDIBackend = ESP32NimBLEBackend;
END_CS_NAMESPACE
#define CS_BLE_MIDI_SUPPORTED 1
#else
// Bluedroid backend (default)
#include "BLEMIDI/ESP32BluedroidBackend.hpp"
BEGIN_CS_NAMESPACE
using BLEMIDIBackend = ESP32BluedroidBackend;
END_CS_NAMESPACE
#define CS_BLE_MIDI_SUPPORTED 1
#endif
#endif

#elif defined(ARDUINO_RASPBERRY_PI_PICO_W)
// Pico W
#if ENABLE_BLE
#include "BLEMIDI/BTstackBackgroundBackend.hpp"
BEGIN_CS_NAMESPACE
using BLEMIDIBackend = BTstackBackgroundBackend;
END_CS_NAMESPACE
#define CS_BLE_MIDI_SUPPORTED 1
#endif

#elif (defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_ARDUINO_NANO33BLE)) ||    \
    (defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_ARCH_RP2040)) ||            \
    (defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_GIGA)) ||                   \
    defined(ARDUINO_UNOR4_WIFI) || defined(ARDUINO_SAMD_NANO_33_IOT) ||        \
    defined(ARDUINO_SAMD_MKRWIFI1010) || defined(CS_USE_ARDUINOBLE)
// Arduino Nano 33 BLE, RP2040, Arduino GIGA, Arduino UNO R4 WiFi,
// Arduino Nano 33 IoT, Arduino MKR 1010 WiFi
#include "BLEMIDI/ArduinoBLEBackend.hpp"
BEGIN_CS_NAMESPACE
using BLEMIDIBackend = ArduinoBLEBackend;
END_CS_NAMESPACE
#define CS_BLE_MIDI_SUPPORTED 1
#endif

#ifdef CS_BLE_MIDI_SUPPORTED
#include "GenericBLEMIDI_Interface.hpp"
BEGIN_CS_NAMESPACE
/// @brief   A class for MIDI interfaces sending MIDI messages over a Bluetooth
///          Low Energy (BLE) connection.
///
/// Configures the Arduino as a BLE peripheral.
///
/// @see @ref md_pages_MIDI-over-USB for a list of supported boards
/// @see @ref md_pages_MIDI-over-BLE for more information and a list of backends
/// @ingroup MIDIInterfaces
struct BluetoothMIDI_Interface : GenericBLEMIDI_Interface<BLEMIDIBackend> {};
END_CS_NAMESPACE
#endif

#ifndef CS_BLE_MIDI_SUPPORTED
#define CS_BLE_MIDI_NOT_SUPPORTED 1
#endif

#endif // CS_BLE_MIDI_NOT_SUPPORTED
