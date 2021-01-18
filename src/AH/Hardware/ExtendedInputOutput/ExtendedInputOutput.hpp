/* ✔ */

#pragma once

#include <AH/Settings/NamespaceSettings.hpp>
#include <AH/Settings/Warnings.hpp>
#include <AH/Hardware/Arduino-Hardware-Types.hpp>

AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

BEGIN_AH_NAMESPACE

class ExtendedIOElement;

/**
 * @brief   A namespace with alternatives to the standard Arduino IO functions
 *          that can be used with extended IO pin numbers.
 */
namespace ExtIO {

/// @addtogroup AH_ExtIO
/// @{

/**
 * @brief   Find the IO element of a given extended IO pin number. 
 * 
 * @param   pin
 *          The extended IO pin number to find the IO element of.
 * @return  A pointer to the extended IO element that the given pin belongs to.
 */
ExtendedIOElement *getIOElementOfPin(pin_t pin);
/// An ExtIO version of the Arduino function
/// @see    ExtendedIOElement::pinMode
void pinMode(pin_t pin, PinMode_t mode);
/// Overload to Arduino pinMode function.
void pinMode(int pin, PinMode_t mode);
/// An ExtIO version of the Arduino function
/// @see    ExtendedIOElement::digitalWrite
void digitalWrite(pin_t pin, PinStatus_t val);
/// Overload to Arduino digitalWrite function.
void digitalWrite(int pin, PinStatus_t val);
/// An ExtIO version of the Arduino function
/// @see    ExtendedIOElement::digitalRead
PinStatus_t digitalRead(pin_t pin);
/// Overload to Arduino digitalRead function.
PinStatus_t digitalRead(int pin);

/// An ExtIO version of the Arduino function
/// @see    ExtendedIOElement::analogRead
analog_t analogRead(pin_t pin);
/// Overload to Arduino analogRead function.
analog_t analogRead(int pin);
/// An ExtIO version of the Arduino function
/// @see    ExtendedIOElement::analogWrite
void analogWrite(pin_t pin, analog_t val);
/// An ExtIO version of the Arduino function
/// @see    ExtendedIOElement::analogWrite
void analogWrite(pin_t pin, int val);
#ifndef ESP32
/// Overload to Arduino analogWrite function.
void analogWrite(int pin, analog_t val);
/// Overload to Arduino analogWrite function.
void analogWrite(int pin, int val);
#endif

/// An ExtIO version of the Arduino function
void shiftOut(pin_t dataPin, pin_t clockPin, BitOrder_t bitOrder, uint8_t val);
/// Overload to Arduino shiftOut function
void shiftOut(int dataPin, int clockPin, BitOrder_t bitOrder, uint8_t val);

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

/// Overload to Arduino pinMode function.
void pinMode(ArduinoPin_t pin, PinMode_t mode);
/// Overload to Arduino digitalWrite function.
void digitalWrite(ArduinoPin_t pin, PinStatus_t val);
/// Overload to Arduino digitalRead function.
PinStatus_t digitalRead(ArduinoPin_t pin);

/// Overload to Arduino analogRead function.
analog_t analogRead(ArduinoPin_t pin);
#ifndef ESP32
/// Overload to Arduino analogWrite function.
void analogWrite(ArduinoPin_t pin, analog_t val);
/// Overload to Arduino analogWrite function.
void analogWrite(ArduinoPin_t pin, int val);
#endif

/// Overload to Arduino shiftOut function
void shiftOut(ArduinoPin_t dataPin, ArduinoPin_t clockPin, BitOrder_t bitOrder, uint8_t val);

/// @}

} // namespace ExtIO

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
