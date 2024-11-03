/* ✔ */

#pragma once

#include <AH/Containers/Array.hpp>
#include <AH/PrintStream/PrintStream.hpp>
#include <AH/STL/limits>
#include <AH/Settings/NamespaceSettings.hpp>
#include <stdint.h> // uint8_t

BEGIN_AH_NAMESPACE

/// The type returned from analogRead and similar functions.
using analog_t = uint16_t;
/// Integer type used internally to store the index of (extended) GPIO pins.
/// This type is also used to represent _offsets_ of pin
/// numbers (e.g. the index of a pin within an extended IO element).
/// In contrast, @ref ExtIO::pin_t represents an _absolute_ pin number.
using pin_int_t = uint_fast16_t;
constexpr pin_int_t NO_PIN_INT =
    (std::numeric_limits<pin_int_t>::max() >> 1) + 1;

namespace ExtIO {
/// Type for storing pin numbers of Extended Input/Output elements.
struct pin_t {
    /// Default constructor (NO_PIN).
    constexpr pin_t() = default;
    /// Constructor from integer.
    constexpr pin_t(pin_int_t pin) : pin(pin) {}

    /// The actual underlying pin number.
    pin_int_t pin = NO_PIN_INT;

    static_assert(std::is_unsigned<decltype(pin)>::value,
                  "Error: pin_t should be an unsigned integer type");

    pin_t &operator+=(pin_int_t b) {
        this->pin += b;
        return *this;
    }
    pin_t &operator++() {
        ++pin;
        return *this;
    }
    pin_t operator++(int) {
        pin_t t = *this;
        ++pin;
        return t;
    }

    pin_t &operator-=(pin_int_t b) {
        this->pin -= b;
        return *this;
    }
    pin_t &operator--() {
        --pin;
        return *this;
    }
    pin_t operator--(int) {
        pin_t t = *this;
        --pin;
        return t;
    }
};
constexpr inline bool operator==(pin_t a, pin_t b) { return a.pin == b.pin; }
constexpr inline bool operator<(pin_t a, pin_t b) { return a.pin < b.pin; }
constexpr inline bool operator<=(pin_t a, pin_t b) { return a.pin <= b.pin; }
constexpr inline bool operator>(pin_t a, pin_t b) { return a.pin > b.pin; }
constexpr inline bool operator>=(pin_t a, pin_t b) { return a.pin >= b.pin; }
constexpr inline bool operator!=(pin_t a, pin_t b) { return !(a == b); }
constexpr inline pin_int_t operator-(pin_t a, pin_t b) { return a.pin - b.pin; }
constexpr inline pin_t operator-(pin_t a, pin_int_t b) { return a.pin - b; }
constexpr inline pin_t operator+(pin_t a, pin_int_t b) { return a.pin + b; }
constexpr inline pin_t operator+(pin_int_t a, pin_t b) { return a + b.pin; }
constexpr inline pin_t operator*(pin_t a, pin_int_t b) { return a.pin * b; }
constexpr inline pin_t operator*(pin_int_t a, pin_t b) { return a * b.pin; }
inline Print &operator<<(Print &os, pin_t p) {
    using AH::operator<<;
    return os << +p.pin;
}
#ifndef ARDUINO
inline std::ostream &operator<<(std::ostream &os, pin_t p) {
    return os << +p.pin;
}
#endif
} // namespace ExtIO

/// The type for Arduino pins (and ExtendedIOElement pins).
using ExtIO::pin_t;

#ifdef NO_PIN // Fix for FastLED: https://github.com/FastLED/FastLED/issues/893
#undef NO_PIN
#endif

/// A special pin number that indicates an unused or invalid pin.
constexpr pin_t NO_PIN {};

/// An easy alias for arrays of pins.
template <size_t N>
using PinList = Array<pin_t, N>;

END_AH_NAMESPACE
