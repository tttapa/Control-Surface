/* ✔ */

#pragma once

#include "Channel.hpp"
#include <AH/Containers/Array.hpp>
#include <AH/Hardware/Hardware-Types.hpp>
#include <Settings/NamespaceSettings.hpp>
#include <stddef.h> // size_t
#include <stdint.h> // uint8_t

BEGIN_CS_NAMESPACE

using AH::analog_t;
using AH::NO_PIN;
using AH::pin_t;
using AH::PinList;

using MappingFunction = analog_t (*)(analog_t);

using AH::Array;
using AH::Array2D;

/// @todo   This should be an array of type MIDICNChannelAddress.
template <uint8_t nb_rows, uint8_t nb_cols>
using AddressMatrix = Array2D<uint8_t, nb_rows, nb_cols>;


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
struct Potentiometer {};
struct MotorFader {};
struct Display {};

/// A simple struct representing a pixel with integer coordinates.
struct PixelLocation {
    int16_t x;
    int16_t y;
};

END_CS_NAMESPACE
