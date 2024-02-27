/* ✔ */

#pragma once

#include <AH/Containers/Array.hpp>
#include <AH/Settings/NamespaceSettings.hpp>
#include <stdint.h> // uint8_t

BEGIN_AH_NAMESPACE

/// The type returned from analogRead and similar functions.
using analog_t = uint16_t;
/// The type for Arduino pins (and ExtendedIOElement pins).
using pin_t = uint16_t;

#ifdef NO_PIN // Fix for FastLED: https://github.com/FastLED/FastLED/issues/893
#undef NO_PIN
#endif

/// A special pin number that indicates an unused or invalid pin.
constexpr pin_t NO_PIN = 1 << (8 * sizeof(pin_t) - 1);

/// An easy alias for arrays of pins.
template <size_t N>
using PinList = Array<pin_t, N>;

END_AH_NAMESPACE
