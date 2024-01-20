#pragma once

#ifdef ESP32
#include <sdkconfig.h>
#if CONFIG_BT_BLE_ENABLED
// ESP32 with BLE support
#ifdef CS_USE_NIMBLE
// NimBLE backend
#include "BLEMIDI/ESP32NimBLEBackend.hpp"
BEGIN_CS_NAMESPACE
using BLEMIDIBackend = ESP32NimBLEBackend;
END_CS_NAMESPACE
#define CS_BLEMIDI_SUPPORTED 1
#else
// Bluedroid backend (default)
#include "BLEMIDI/ESP32BluedroidBackend.hpp"
BEGIN_CS_NAMESPACE
using BLEMIDIBackend = ESP32BluedroidBackend;
END_CS_NAMESPACE
#define CS_BLEMIDI_SUPPORTED 1
#endif
#endif
#elif (defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_ARDUINO_NANO33BLE)) ||    \
    (defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_ARCH_RP2040)) ||            \
    (defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_GIGA)) ||                   \
    defined(ARDUINO_UNOR4_WIFI) || defined(ARDUINO_SAMD_NANO_33_IOT) ||        \
    defined(ARDUINO_SAMD_MKRWIFI1010)
// Arduino Nano 33 BLE, RP2040, Arduino UNO R4 WiFi, Arduino Nano 33 IoT
#include "BLEMIDI/ArduinoBLEBackend.hpp"
BEGIN_CS_NAMESPACE
using BLEMIDIBackend = ArduinoBLEBackend;
END_CS_NAMESPACE
#define CS_BLEMIDI_SUPPORTED 1
#endif

#ifdef CS_BLEMIDI_SUPPORTED
#include "GenericBLEMIDI_Interface.hpp"
BEGIN_CS_NAMESPACE
using BluetoothMIDI_Interface = GenericBLEMIDI_Interface<BLEMIDIBackend>;
END_CS_NAMESPACE
#endif
