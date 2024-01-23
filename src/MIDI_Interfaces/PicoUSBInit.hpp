#pragma once

/**
 * @file
 * When using the earlephilhower/arduino-pico core with the TinyUSB backend,
 * calling Serial.read/print/write before TinyUSBDevice.mounted() returns true
 * crashes the chip. The function in this file checks if the given stream is
 * actually the `Serial` port (USB CDC), and if so returns whether TinyUSB
 * has been mounted or not. If it returns true, it is safe to read/write from/to
 * the stream.
 */

#include <AH/Arduino-Wrapper.h>

#if defined(ARDUINO_ARCH_RP2040) && !defined(ARDUINO_ARCH_MBED) &&             \
    defined(USE_TINYUSB)
#include <Adafruit_TinyUSB.h>
namespace {
bool ensure_usb_init(Print &stream) {
    if (&stream == static_cast<Print *>(&Serial))
        return TinyUSBDevice.mounted();
    return true;
}
} // namespace
#else
namespace {
constexpr bool ensure_usb_init(Print &) { return true; }
} // namespace
#endif
