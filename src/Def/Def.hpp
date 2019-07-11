/* ✔ */

#pragma once

#include "Channel.hpp"
#include "Frequency.hpp"
#include <Helpers/Array.hpp>
#include <stddef.h> // size_t
#include <stdint.h> // uint8_t

/// The type returned from analogRead and similar functions.
typedef int16_t analog_t;
/// The type for Arduino pins (and ExtendedIOElement pins).
typedef uint16_t pin_t;
/// A special pin number that indicates an unused or invalid pin.
constexpr pin_t NO_PIN = 1 << (8 * sizeof(pin_t) - 1);

/// A function pointer to a mapping function to map analog values.
/// @see    MIDIFilteredAnalog::map()
using MappingFunction = uint8_t (*)(uint8_t);

/// An easy alias for two-dimensional Arrays.
template <class T, size_t nb_rows, size_t nb_cols>
using Array2D = Array<Array<T, nb_cols>, nb_rows>;

/// A special address that indicates an unused or invalid address.
constexpr uint8_t NO_ADDRESS = {0x80};
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

class MIDICNChannelAddress;

/// A pointer to functions sending on/off MIDI events.
using DigitalSendFunction = void (*)(MIDICNChannelAddress);
/// A pointer to functions sending 7-bit continuous MIDI events.
// value, address
using ContinuousSendFunction7Bit = void (*)(uint8_t, MIDICNChannelAddress);
/// A pointer to functions sending 14-bit continuous MIDI events.
// value, address
using ContinuousSendFunction14Bit = void (*)(uint16_t, MIDICNChannelAddress);
/// A pointer to functions sending relative MIDI events.
// delta, address
using RelativeSendFunction = void (*)(long, MIDICNChannelAddress);

/// The type used for Selector#s.
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