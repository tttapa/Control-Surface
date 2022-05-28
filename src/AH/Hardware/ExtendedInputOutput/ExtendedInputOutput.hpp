/* ✔ */

#pragma once

#include <AH/Hardware/Arduino-Hardware-Types.hpp>
#include <AH/STL/climits>
#include <AH/Settings/NamespaceSettings.hpp>

BEGIN_AH_NAMESPACE

class ExtendedIOElement;

/**
 * @brief   A namespace with alternatives to the standard Arduino IO functions
 *          that can be used with extended IO pin numbers.
 */
namespace ExtIO {

/// @addtogroup AH_ExtIO
/// @{

/// Check if the given pin number is a real Arduino pin number, and not an ExtIO
/// pin number.
inline bool isNativePin(pin_t pin) {
    return pin.pin < NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS;
}

/**
 * @brief   Find the IO element of a given extended IO pin number. 
 * 
 * @param   pin
 *          The extended IO pin number to find the IO element of.
 * @return  A pointer to the extended IO element that the given pin belongs to.
 */
ExtendedIOElement *getIOElementOfPinOrNull(pin_t pin);
/// @copydoc getIOElementOfPinOrNull
/// Throws an error if the element was not found.
ExtendedIOElement *getIOElementOfPin(pin_t pin);

/// An ExtIO version of the Arduino function
/// @see    ExtendedIOElement::pinMode
void pinMode(pin_t pin, PinMode_t mode);
/// An ExtIO version of the Arduino function
/// @see    ExtendedIOElement::digitalWrite
void digitalWrite(pin_t pin, PinStatus_t val);
/// An ExtIO version of the Arduino function
/// @see    ExtendedIOElement::digitalRead
PinStatus_t digitalRead(pin_t pin);

/// An ExtIO version of the Arduino function
/// @see    ExtendedIOElement::analogRead
analog_t analogRead(pin_t pin);
/// An ExtIO version of the Arduino function
/// @see    ExtendedIOElement::analogWrite
void analogWrite(pin_t pin, analog_t val);
/// An ExtIO version of the Arduino function
/// @see    ExtendedIOElement::analogWrite
void analogWrite(pin_t pin, int val);

/// An ExtIO version of the Arduino function
void shiftOut(pin_t dataPin, pin_t clockPin, BitOrder_t bitOrder, uint8_t val);

/// A buffered ExtIO version of the Arduino function
/// @see   ExtendedIOElement::pinModeBuffered
void pinModeBuffered(pin_t pin, PinMode_t mode);
/// A buffered ExtIO version of the Arduino function
/// @see   ExtendedIOElement::digitalWriteBuffered
void digitalWriteBuffered(pin_t pin, PinStatus_t val);
/// A buffered ExtIO version of the Arduino function
/// @see   ExtendedIOElement::digitalReadBuffered
PinStatus_t digitalReadBuffered(pin_t pin);

/// A buffered ExtIO version of the Arduino function
/// @see   ExtendedIOElement::analogReadBuffered
analog_t analogReadBuffered(pin_t pin);
/// A buffered ExtIO version of the Arduino function
/// @see   ExtendedIOElement::analogWriteBuffered
void analogWriteBuffered(pin_t pin, analog_t val);
/// A buffered ExtIO version of the Arduino function
/// @see   ExtendedIOElement::analogWriteBuffered
void analogWriteBuffered(pin_t pin, int val);

/// @}

} // namespace ExtIO

END_AH_NAMESPACE
