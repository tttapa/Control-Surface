#pragma once

#include <AH/Containers/Array.hpp>
#include <AH/STL/cstdint>
#include <Settings/NamespaceSettings.hpp>

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

END_CS_NAMESPACE

#ifdef ARDUINO

#if defined(TEENSYDUINO)

#if defined(__AVR__) /* Teensy 2.x */
#include "USBMIDI_Teensy2.hpp"

#elif defined(__MK20DX128__)  /* Teensy 3.0 */                                 \
    || defined(__MK20DX256__) /* Teensy 3.1/3.2 */                             \
    || defined(__MK64FX512__) /* Teensy 3.5 */                                 \
    || defined(__MK66FX1M0__) /* Teensy 3.6 */
#include "USBMIDI_Teensy3.hpp"

#elif defined(__IMXRT1062__) || defined(__IMXRT1052__) /* Teensy 4.0 */
#include "USBMIDI_Teensy4.hpp"

#elif defined(__MKL26Z64__) /* Teensy LC */
#include "USBMIDI_TeensyLC.hpp"

#else
#warning "Unknown Teensy board, please open an issue on GitHub" \
         "https://github.com/tttapa/Arduino-Helpers"
// Fall back to the MIDIUSB library and hope for the best
#include "USBMIDI_MIDIUSB.hpp"
#endif

#elif defined(ARDUINO_ARCH_MBED)

#include "USBMIDI_Arduino_mbed.hpp"

#else

#include <AH/Arduino-Wrapper.h>
#ifdef USBCON
#include "USBMIDI_MIDIUSB.hpp"
#else
#define CS_USB_MIDI_NOT_SUPPORTED
#endif

#endif

#else

#include "USBMIDI_Mock.hpp"

#endif
