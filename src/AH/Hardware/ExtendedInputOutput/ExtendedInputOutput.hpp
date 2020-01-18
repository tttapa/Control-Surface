/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>
#include <AH/Settings/NamespaceSettings.hpp>

AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

AH_DIAGNOSTIC_EXTERNAL_HEADER()
#include <AH/Arduino-Wrapper.h> // pin functions and constants
AH_DIAGNOSTIC_POP()

#include <AH/Hardware/Hardware-Types.hpp>

BEGIN_AH_NAMESPACE

#define AH_EXT_PIN(x) (x + NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS)

namespace detail {
constexpr static auto tmp_HIGH = HIGH;
constexpr static auto tmp_LOW = LOW;
constexpr static auto tmp_INPUT = INPUT;
constexpr static auto tmp_OUTPUT = OUTPUT;
constexpr static auto tmp_INPUT_PULLUP = INPUT_PULLUP;
} // namespace detail

END_AH_NAMESPACE

#ifndef ARDUINO_API_VERSION
#ifdef HIGH
#undef HIGH
#endif
#ifdef LOW
#undef LOW
#endif

#ifdef INPUT
#undef INPUT
#endif
#ifdef OUTPUT
#undef OUTPUT
#endif
#ifdef INPUT_PULLUP
#undef INPUT_PULLUP
#endif

using PinStatus_t = uint8_t;
using PinMode_t = uint8_t;
#if defined(SAMD_SERIES) /* Nano 33 */ || defined(_LIB_SAM_) /* Due */
using BitOrder_t = BitOrder;
#else 
using BitOrder_t = uint8_t;
#endif

const PinStatus_t HIGH = AH::detail::tmp_HIGH;
const PinStatus_t LOW = AH::detail::tmp_LOW;

const PinMode_t INPUT = AH::detail::tmp_INPUT;
const PinMode_t OUTPUT = AH::detail::tmp_OUTPUT;
const PinMode_t INPUT_PULLUP = AH::detail::tmp_INPUT_PULLUP;

#else
using PinStatus_t = PinStatus;
using PinMode_t = PinMode;
using BitOrder_t = BitOrder;
#endif

BEGIN_AH_NAMESPACE

class ExtendedIOElement;

/**
 * @brief   A namespace with alternatives to the standard Arduino IO functions
 *          that can be used with extended IO pin numbers.
 */
namespace ExtIO {
/**
 * @brief   Find the IO element of a given extended IO pin number. 
 * 
 * @param   pin
 *          The extended IO pin number to find the IO element of.
 * @return  A pointer to the extended IO element that the given pin belongs to.
 */
ExtendedIOElement &getIOElementOfPin(pin_t pin);
/// An ExtIO version of the Arduino function
void pinMode(pin_t pin, PinMode_t mode);
/// An ExtIO version of the Arduino function
void pinMode(int pin, PinMode_t mode);
/// An ExtIO version of the Arduino function
void digitalWrite(pin_t pin, PinStatus_t val);
/// An ExtIO version of the Arduino function
void digitalWrite(int pin, PinStatus_t val);
/// An ExtIO version of the Arduino function
int digitalRead(pin_t pin);
/// An ExtIO version of the Arduino function
int digitalRead(int pin);
/// An ExtIO version of the Arduino function
void shiftOut(pin_t dataPin, pin_t clockPin, BitOrder_t bitOrder,
                     uint8_t val);
/// An ExtIO version of the Arduino function
void shiftOut(int dataPin, int clockPin, BitOrder_t bitOrder,
                     uint8_t val);
/// An ExtIO version of the Arduino function
analog_t analogRead(pin_t pin);
/// An ExtIO version of the Arduino function
analog_t analogRead(int pin);
/// An ExtIO version of the Arduino function
void analogWrite(pin_t pin, analog_t val);
/// An ExtIO version of the Arduino function
void analogWrite(int pin, analog_t val);
/// An ExtIO version of the Arduino function
void analogWrite(int pin, int val);
/// An ExtIO version of the Arduino function
void analogWrite(pin_t pin, int val);

} // namespace ExtIO

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
