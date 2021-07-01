/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include "ExtendedInputOutput.hpp"
#include <AH/Containers/Updatable.hpp>
#include <AH/Hardware/Hardware-Types.hpp>

BEGIN_AH_NAMESPACE

/**
 * @brief   An abstract base class for Extended Input/Output elements.
 *
 * The limited number of IO pins of the Arduino can be extended by
 * adding multiplexers, shift registers, IO expanders, etc.  
 * ExtendedIOElement serves as a base class for all these expanders.
 *
 * The pins of each extended IO element are mapped to a pin number
 * greater than the greatest Arduino pin number.  
 * You can supply this pin number to the IO functions in the ExtIO
 * namespace.  
 * If the pin number corresponds to an actual Arduino pin,
 * the default Arduino IO function (digitalRead, digitalWrite, ...)
 * will be called.  
 * If the pin is not an Arduino pin, it is an extended IO pin number,
 * so the extended IO element that this pin belongs to is looked up,
 * and the IO function for this element is executed with the correct
 * pin number.
 *
 * For example:
 * Imagine an Arduino with 20 pins (e.g. the Arduino UNO).
 * Pins 0 - 19 will correspond to the Arduino pins, and
 * `ExtIO::digitalRead(pin)` will have the exact same effect as
 * the standard `digitalRead(pin)` function (albeit a little slower).  
 * Now, we'll add two 8-channel analog multiplexers, let's call them
 * `mux1` and `mux2`.  
 * The first pin (pin 0) of `mux1` will be extended IO pin number 20,
 * the last pin (pin 7) of `mux1` will be extended IO pin number 27,
 * etc.
 * The first pin of `mux2` will be extended IO pin number 28, you get
 * the idea.
 * If you now call `ExtIO::digitalRead(mux1.#pin (7))` or
 * `ExtIO::digitalRead(27)`, both will be
 * translated to `mux1.digitalRead(7)`.
 *
 * The number of extended IO elements is limited only by the size of
 * `pin_t`. However, looking up the extended IO element for a given
 * extended IO pin number uses linear search, so that might add
 * some noticable overhead for large pin numbers.  
 * 
 * The design here is a compromise: saving a pointer to each extended IO element
 * to find it directly would be much faster than having to search all elements
 * each time. On the other hand, it would require each `pin_t` variable to be
 * at least one byte larger. Since almost all other classes in this library 
 * store pin variables, the memory penalty would be too large, especially on AVR
 * microcontrollers.  
 * Another reason to do it this way, is that this approach is still fast enough
 * to make sure it is not noticable to human users.
 */
class ExtendedIOElement : public UpdatableCRTP<ExtendedIOElement> {
  protected:
    /**
     * @brief   Create an ExtendedIOElement with the given number of pins.
     * 
     * @param   length
     *          The number of pins this element has.
     */
    ExtendedIOElement(pin_t length);

    /// Copying not allowed.
    ExtendedIOElement(const ExtendedIOElement &) = delete;
    /// Copying not allowed.
    ExtendedIOElement &operator=(const ExtendedIOElement &) = delete;

    /// Move constructor.
    ExtendedIOElement(ExtendedIOElement &&) = default;
    /// Move assignment.
    ExtendedIOElement &operator=(ExtendedIOElement &&) = delete;

  public:
    /** 
     * @brief   Set the mode of a given pin.
     * 
     * @note    This function might not be implemented by all subclasses.  
     *          Some extended IO types, such as shift registers, can only be 
     *          used as outputs.  
     *          On others, it might be implemented, but it could impact all pins
     *          of the IO element. For example, enabling the internal pull-up 
     *          resistor on an analog multiplexer affects all pins of the mux.
     * 
     * @param   pin
     *          The (zero-based) pin of this IO element.
     * @param   mode
     *          The mode to set the pin to (e.g. `INPUT`, `OUTPUT` or 
     *          `INPUT_PULLUP`).
     */
    virtual void pinMode(pin_t pin, PinMode_t mode) {
        pinModeBuffered(pin, mode);
        updateBufferedOutputs();
    }

    /**
     * @brief   Set the mode of a given pin in the software buffer.
     * The buffer is written to the ExtIO device when @ref updateBufferedOutputs
     * is called.
     * @copydetails pinMode
     */
    virtual void pinModeBuffered(pin_t pin, PinMode_t mode) = 0;

    /** 
     * @brief   Set the output of the given pin to the given state.
     * 
     * @param   pin
     *          The (zero-based) pin of this IO element.
     * @param   state
     *          The new state to set the pin to.
     */
    virtual void digitalWrite(pin_t pin, PinStatus_t state) {
        digitalWriteBuffered(pin, state);
        updateBufferedOutputs();
    }

    /**
     * @brief   Set the output of a given pin in the software buffer.
     * The buffer is written to the ExtIO device when @ref updateBufferedOutputs
     * is called.
     * @copydetails digitalWrite
     */
    virtual void digitalWriteBuffered(pin_t pin, PinStatus_t state) = 0;

    /** 
     * @brief   Read the state of the given pin.
     * 
     * @param   pin
     *          The (zero-based) pin of this IO element.
     * @return  The state of the given pin.
     */
    virtual PinStatus_t digitalRead(pin_t pin) {
        updateBufferedInputs();
        return digitalReadBuffered(pin);
    }

    /** 
     * @brief   Read the state of the given pin from the software buffer.
     * To update the buffer, you have to call @ref updateBufferedInputs first.
     * @copydetails digitalRead
     */
    virtual PinStatus_t digitalReadBuffered(pin_t pin) = 0;

    /**
     * @brief   Write an analog (or PWM) value to the given pin.
     * 
     * @param   pin 
     *          The (zero-based) pin of this IO element.
     * @param   val 
     *          The new analog value to set the pin to.
     */
    virtual void analogWrite(pin_t pin, analog_t val) {
        analogWriteBuffered(pin, val);
        updateBufferedOutputs();
    }

    /**
     * @brief   Write an analog (or PWM) value to the software buffer given pin.
     * The buffer is written to the ExtIO device when @ref updateBufferedOutputs
     * is called.
     * @copydetails analogWrite
     */
    virtual void analogWriteBuffered(pin_t pin, analog_t val) = 0;

    /**
     * @brief   Read the analog value of the given pin.
     * 
     * @param   pin 
     *          The (zero-based) pin of this IO element.
     * @return  The new analog value of pin.
     */
    virtual analog_t analogRead(pin_t pin) {
        updateBufferedInputs();
        return analogReadBuffered(pin);
    }

    /** 
     * @brief   Read the analog value of the given pin from the software buffer.
     * To update the buffer, you have to call @ref updateBufferedInputs first.
     * @copydetails analogRead
     */
    virtual analog_t analogReadBuffered(pin_t pin) = 0;

    /**
     * @brief   Initialize the extended IO element.
     */
    virtual void begin() = 0;

    /**
     * @brief   Initialize all extended IO elements.
     */
    static void beginAll();

    /**
     * @brief   Write the internal state to the physical outputs.
     */
    virtual void updateBufferedOutputs() = 0;

    /**
     * @brief   Write the internal states to the physical outputs for all 
     *          extended IO elements.
     */
    static void updateAllBufferedOutputs();

    /** 
     * @brief   Read the physical state into the input buffers.
     */
    virtual void updateBufferedInputs() = 0;

    /** 
     * @brief   Read the physical state into the input buffers for all extended
     *          IO elements.
     */
    static void updateAllBufferedInputs();

    /**
     * @brief   Get the extended IO pin number of a given physical pin of this
     *          extended IO element.
     * @param   pin
     *          The zero-based physical pin number of this IO element.
     * @return  The global, unique extended IO pin number for the given pin.
     */
    pin_t pin(pin_t pin) const;

    /**
     * @brief   Get the extended IO pin number of a given physical pin of this
     *          extended IO element.  
     *          It is alias for `ExtendedIOElement::pin`.
     * @param   pin
     *          The zero-based physical pin number of this IO element.
     * @return  The global, unique extended IO pin number for the given pin.
     */
    pin_t operator[](pin_t pin) const;

    /**
     * @brief Get the number of pins this IO element has.
     * 
     * @return The number of pins this IO element has. 
     */
    pin_t getLength() const;

    /**
     * @brief   Get the largest global extended IO pin number that belongs to
     *          this extended IO element.
     */
    pin_t getEnd() const;

    /**
     * @brief   Get the smallest global extended IO pin number that belongs to
     *          this extended IO element.
     */
    pin_t getStart() const;

    /**
     * @brief   Get the list of all Extended IO elements.
     */
    static DoublyLinkedList<ExtendedIOElement> &getAll();

  private:
    const pin_t length;
    const pin_t start;
    const pin_t end;
    static pin_t offset;
};

namespace ExtIO {

struct CachedExtIOPin {
    explicit CachedExtIOPin(pin_t pin)
        : element(pin == NO_PIN || isNativePin(pin) ? nullptr
                                                    : getIOElementOfPin(pin)),
          elementPin(element ? pin - element->getStart() : pin) {}

    template <class FRet, class... FArgs, class Fallback>
    FRet __attribute__((always_inline))
    apply(FRet (ExtendedIOElement::*func)(pin_t, FArgs...), Fallback &&fallback,
          FArgs... args) {
        if (element != nullptr)
            return (element->*func)(elementPin, args...);
        else if (elementPin != NO_PIN)
            return fallback(arduino_pin_cast(elementPin), args...);
        else
            return static_cast<FRet>(0);
    }

    ExtendedIOElement *element;
    pin_t elementPin;
};

/// An ExtIO version of the Arduino function
/// @see    ExtendedIOElement::pinMode
inline void pinMode(CachedExtIOPin pin, PinMode_t mode) {
    pin.apply(
        &ExtendedIOElement::pinMode, //
        [](ArduinoPin_t p, PinMode_t m) { ::pinMode(p, m); }, mode);
}
/// An ExtIO version of the Arduino function
/// @see    ExtendedIOElement::digitalWrite
inline void digitalWrite(CachedExtIOPin pin, PinStatus_t val) {
    pin.apply(
        &ExtendedIOElement::digitalWrite, //
        [](ArduinoPin_t p, PinStatus_t v) { ::digitalWrite(p, v); }, val);
}
/// An ExtIO version of the Arduino function
/// @see    ExtendedIOElement::digitalRead
inline PinStatus_t digitalRead(CachedExtIOPin pin) {
    return pin.apply(&ExtendedIOElement::digitalRead, //
                     [](ArduinoPin_t p) { return ::digitalRead(p); });
}

/// An ExtIO version of the Arduino function
/// @see    ExtendedIOElement::analogRead
inline analog_t analogRead(CachedExtIOPin pin) {
    return pin.apply(&ExtendedIOElement::analogRead, //
                     [](ArduinoPin_t p) { return ::analogRead(p); });
}
/// An ExtIO version of the Arduino function
/// @see    ExtendedIOElement::analogWrite
inline void analogWrite(CachedExtIOPin pin, analog_t val) {
#ifndef ESP32
    pin.apply(
        &ExtendedIOElement::analogWrite, //
        [](ArduinoPin_t p, analog_t v) { ::analogWrite(p, v); }, val);
#else
    pin.apply(
        &ExtendedIOElement::analogWrite, //
        [](ArduinoPin_t, analog_t) {}, val);
#endif
}
/// An ExtIO version of the Arduino function
/// @see    ExtendedIOElement::analogWrite
inline void analogWrite(CachedExtIOPin pin, int val) {
    return analogWrite(pin, static_cast<analog_t>(val));
}

/// An ExtIO version of the Arduino function
inline void shiftOut(CachedExtIOPin dataPin, CachedExtIOPin clockPin,
                     BitOrder_t bitOrder, uint8_t val) {
    if (dataPin.elementPin == NO_PIN || clockPin.elementPin == NO_PIN)
        return;
    // Native version
    if (dataPin.element == nullptr && clockPin.element == nullptr) {
        ::shiftOut(arduino_pin_cast(dataPin.elementPin),
                   arduino_pin_cast(clockPin.elementPin), bitOrder, val);
    }
    // ExtIO version
    else if (dataPin.element != nullptr && clockPin.element != nullptr) {
        const auto dataEl = dataPin.element;
        const auto dataPinN = dataPin.elementPin;
        const auto clockEl = clockPin.element;
        const auto clockPinN = clockPin.elementPin;
        for (uint8_t i = 0; i < 8; i++) {
            uint8_t mask = bitOrder == LSBFIRST ? (1 << i) : (1 << (7 - i));
            dataEl->digitalWrite(dataPinN, (val & mask) ? HIGH : LOW);
            clockEl->digitalWrite(clockPinN, HIGH);
            clockEl->digitalWrite(clockPinN, LOW);
        }
    }
    // Mixed version (slow)
    else {
        for (uint8_t i = 0; i < 8; i++) {
            uint8_t mask = bitOrder == LSBFIRST ? (1 << i) : (1 << (7 - i));
            digitalWrite(dataPin, (val & mask) ? HIGH : LOW);
            digitalWrite(clockPin, HIGH);
            digitalWrite(clockPin, LOW);
        }
    }
}

/// A buffered ExtIO version of the Arduino function
/// @see    ExtendedIOElement::pinModeBuffered
inline void pinModeBuffered(CachedExtIOPin pin, PinMode_t mode) {
    pin.apply(
        &ExtendedIOElement::pinModeBuffered, //
        [](ArduinoPin_t p, PinMode_t m) { ::pinMode(p, m); }, mode);
}
/// A buffered ExtIO version of the Arduino function
/// @see    ExtendedIOElement::digitalWriteBuffered
inline void digitalWriteBuffered(CachedExtIOPin pin, PinStatus_t val) {
    pin.apply(
        &ExtendedIOElement::digitalWriteBuffered, //
        [](ArduinoPin_t p, PinStatus_t v) { ::digitalWrite(p, v); }, val);
}
/// A buffered ExtIO version of the Arduino function
/// @see    ExtendedIOElement::digitalReadBuffered
inline PinStatus_t digitalReadBuffered(CachedExtIOPin pin) {
    return pin.apply(&ExtendedIOElement::digitalReadBuffered, //
                     [](ArduinoPin_t p) { return ::digitalRead(p); });
}

/// A buffered ExtIO version of the Arduino function
/// @see    ExtendedIOElement::analogReadBuffered
inline analog_t analogReadBuffered(CachedExtIOPin pin) {
    return pin.apply(&ExtendedIOElement::analogReadBuffered, //
                     [](ArduinoPin_t p) { return ::analogRead(p); });
}
/// A buffered ExtIO version of the Arduino function
/// @see    ExtendedIOElement::analogWriteBuffered
inline void analogWriteBuffered(CachedExtIOPin pin, analog_t val) {
#ifndef ESP32
    pin.apply(
        &ExtendedIOElement::analogWriteBuffered, //
        [](ArduinoPin_t p, analog_t v) { ::analogWrite(p, v); }, val);
#else
    pin.apply(
        &ExtendedIOElement::analogWriteBuffered, //
        [](ArduinoPin_t, analog_t) {}, val);
#endif
}
/// A buffered ExtIO version of the Arduino function
/// @see    ExtendedIOElement::analogWriteBuffered
inline void analogWriteBuffered(CachedExtIOPin pin, int val) {
    return analogWrite(pin, static_cast<analog_t>(val));
}
} // namespace ExtIO

using ExtIO::CachedExtIOPin;

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
