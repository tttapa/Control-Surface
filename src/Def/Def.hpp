/* ✔ */

#pragma once

#include "Channel.hpp"
#include "Frequency.hpp"
#include <Helpers/Array.hpp>
#include <Settings/NamespaceSettings.hpp>
#include <stddef.h> // size_t
#include <stdint.h> // uint8_t

BEGIN_CS_NAMESPACE

/// The bit depth of the ADC (Analog to Digital Converter).
constexpr static uint8_t ADC_BITS = 10;

/// The type returned from analogRead and similar functions.
using analog_t = uint16_t;
/// The type for Arduino pins (and ExtendedIOElement pins).
using pin_t = uint16_t;

#ifdef NO_PIN // Fix for FastLED: https://github.com/FastLED/FastLED/issues/893
#undef NO_PIN
#endif

/// A special pin number that indicates an unused or invalid pin.
constexpr pin_t NO_PIN = 1 << (8 * sizeof(pin_t) - 1);

/// A function pointer to a mapping function to map analog values.
/// @see    MIDIFilteredAnalog::map()
using MappingFunction = analog_t (*)(analog_t);

/// An easy alias for two-dimensional Arrays.
template <class T, size_t nb_rows, size_t nb_cols>
using Array2D = Array<Array<T, nb_cols>, nb_rows>;

/// @todo   This should be an array of type MIDICNChannelAddress.
template <uint8_t nb_rows, uint8_t nb_cols>
using AddressMatrix = Array2D<uint8_t, nb_rows, nb_cols>;

/// An easy alias for arrays of pins.
template <size_t N>
using PinList = Array<pin_t, N>;

/// A struct for the pins of a rotary (quadrature) encoder with a switch.
struct EncoderSwitchPinList {
    // TODO: why do I need explicit constructors?
    EncoderSwitchPinList(uint8_t A, uint8_t B, pin_t switchPin)
        : A(A), B(B), switchPin(switchPin) {}
    EncoderSwitchPinList(uint8_t A, uint8_t B)
        : A(A), B(B), switchPin(NO_PIN) {}

    uint8_t A; ///< The pin connected to the A pin of the encoder.
    uint8_t B; ///< The pin connected to the B pin of the encoder.
    pin_t switchPin = NO_PIN; ///< The pin connected to the switch pin of the
                              ///< encoder.
};

/// A struct for the pins of a rotary (quadrature) encoder without a switch.
struct EncoderPinList {
    uint8_t A; ///< The pin connected to the A pin of the encoder.
    uint8_t B; ///< The pin connected to the B pin of the encoder.
};

/// The type used for Selector%s.
using setting_t = uint8_t;
/// A special setting that indicates an unused or invalid setting.
constexpr setting_t NO_SETTING = 1 << (8 * sizeof(setting_t) - 1);

// Updatable types:
struct NormalUpdatable {};
struct Potentiometer {};
struct MotorFader {};
struct Display {};

/// A simple struct representing a pixel with integer coordinates.
struct PixelLocation {
    int16_t x;
    int16_t y;
};

#define UNUSED_PARAM __attribute__((unused))

END_CS_NAMESPACE
