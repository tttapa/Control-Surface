#pragma once

#include <AH/Hardware/Hardware-Types.hpp>
#include <AH/Types/FunctionTraits.hpp>

#include <AH/Arduino-Wrapper.h> // pin functions and constants
#include <AH/STL/type_traits>

#if defined(BOARD_HAS_PIN_REMAP) && !defined(BOARD_USES_HW_GPIO_NUMBERS)
#error                                                                         \
    "This library is not compatible with pin remapping. Please disable it by choosing "Tools > Pin Numbering > By GPIO number (legacy)" in the Arduino IDE. See https://github.com/espressif/arduino-esp32/issues/9150 for details"
#endif

#if defined(ARDUINO_API_VERSION)

using ArduinoPin_t = pin_size_t;
using PinStatus_t = PinStatus;
using PinMode_t = PinMode;
using BitOrder_t = BitOrder;

#else // ARDUINO_API_VERSION

using ArduinoPin_t =
    AH::function_traits<decltype(::digitalWrite)>::argument_t<0>;
using PinStatus_t =
    AH::function_traits<decltype(::digitalWrite)>::argument_t<1>;
using PinMode_t = AH::function_traits<decltype(::pinMode)>::argument_t<1>;

#if defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_SAMD)
using BitOrder_t = BitOrder;
#else
using BitOrder_t = uint8_t;
#endif

namespace AH_pin_detail {
constexpr static auto tmp_HIGH = HIGH;
constexpr static auto tmp_LOW = LOW;
constexpr static auto tmp_INPUT = INPUT;
constexpr static auto tmp_OUTPUT = OUTPUT;
constexpr static auto tmp_INPUT_PULLUP = INPUT_PULLUP;
} // namespace AH_pin_detail
#ifdef HIGH
#undef HIGH
#define HIGH HIGH
#endif
#ifdef LOW
#undef LOW
#define LOW LOW
#endif
#ifdef INPUT
#undef INPUT
#define INPUT INPUT
#endif
#ifdef OUTPUT
#undef OUTPUT
#define OUTPUT OUTPUT
#endif
#ifdef INPUT_PULLUP
#undef INPUT_PULLUP
#define INPUT_PULLUP INPUT_PULLUP
#endif
constexpr PinStatus_t HIGH = AH_pin_detail::tmp_HIGH;
constexpr PinStatus_t LOW = AH_pin_detail::tmp_LOW;
constexpr PinMode_t INPUT = AH_pin_detail::tmp_INPUT;
constexpr PinMode_t OUTPUT = AH_pin_detail::tmp_OUTPUT;
constexpr PinMode_t INPUT_PULLUP = AH_pin_detail::tmp_INPUT_PULLUP;

#endif // ARDUINO_API_VERSION

BEGIN_AH_NAMESPACE

template <class T>
constexpr ArduinoPin_t arduino_pin_cast(T t) {
    return static_cast<ArduinoPin_t>(t);
}
constexpr ArduinoPin_t arduino_pin_cast(pin_t t) { return t.pin; }
#if (defined(NOT_AN_INTERRUPT) || defined(ARDUINO_API_VERSION)) &&             \
    !defined(ARDUINO_ARCH_RENESAS)
using not_an_interrupt_t = decltype(NOT_AN_INTERRUPT);
/// Type of interrupt indices (result of digitalPinToInterrupt).
using interrupt_t =
    std::conditional<std::is_enum<not_an_interrupt_t>::value,
                     std::underlying_type<not_an_interrupt_t>,
                     type_identity<not_an_interrupt_t>>::type::type;
#endif

END_AH_NAMESPACE
