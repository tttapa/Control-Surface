#pragma once

/**
 * @file
 * @brief   This file contains the platform-specific ADC resolutions.
 *          By default, the library automatically selects the maximum supported
 *          resolution for known boards, otherwise, it falls back to 10 bits.
 * @see     @ref AH::ADC_BITS
 */

#include <AH/Arduino-Wrapper.h>

#if defined(ADC_RESOLUTION)
#define HAS_ANALOG_READ_RESOLUTION 1

// Teensy
//------------------------------------------------------------------------------
#elif defined(TEENSYDUINO) && !defined(DOXYGEN)

#if defined(__AVR__) // Teensy 2.x
#define ADC_RESOLUTION 10
#define HAS_ANALOG_READ_RESOLUTION 0

#elif defined(__MK20DX128__) // Teensy 3.0
#define ADC_RESOLUTION 13
#define HAS_ANALOG_READ_RESOLUTION 1

#elif defined(__MK20DX256__) // Teensy 3.1/3.2
#define ADC_RESOLUTION 13
#define HAS_ANALOG_READ_RESOLUTION 1

#elif defined(__MKL26Z64__) // Teensy LC
#define ADC_RESOLUTION 12
#define HAS_ANALOG_READ_RESOLUTION 1

#elif defined(__MK64FX512__) // Teensy 3.5
#define ADC_RESOLUTION 13
#define HAS_ANALOG_READ_RESOLUTION 1

#elif defined(__MK66FX1M0__) // Teensy 3.6
#define ADC_RESOLUTION 13
#define HAS_ANALOG_READ_RESOLUTION 1

#elif defined(__IMXRT1062__) || defined(__IMXRT1052__) // Teensy 4.0
#define ADC_RESOLUTION 12
#define HAS_ANALOG_READ_RESOLUTION 1

#else
#warning "Unknown Teensy board, please open an issue on GitHub" \
         "https://github.com/tttapa/Arduino-Helpers"
#endif

// ESP32
//------------------------------------------------------------------------------
#elif defined(ESP32)

#define ADC_RESOLUTION 12
#define HAS_ANALOG_READ_RESOLUTION 1

// Unit test on PC
// -----------------------------------------------------------------------------
#elif !defined(ARDUINO)

#define ADC_RESOLUTION 10
#define HAS_ANALOG_READ_RESOLUTION 1

// Unknown/Default
//------------------------------------------------------------------------------
#else
/// The actual maximum resolution of the built-in ADC.
#define ADC_RESOLUTION 10
/// Whether the platform supports the `analogReadResolution` function.
#define HAS_ANALOG_READ_RESOLUTION 0

#endif
