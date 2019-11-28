/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include "StaticSizeExtendedIOElement.hpp"
#include <AH/Containers/BitArray.hpp>

BEGIN_AH_NAMESPACE

/**
 * @brief   A class for serial-in/parallel-out shift registers, 
 *          like the 74HC595.
 * 
 * @tparam  N
 *          The number of bits in total. Usually, shift registers (e.g. the
 *          74HC595) have eight bits per chip, so `length = 8 * k` where `k`
 *          is the number of cascaded chips.
 * 
 * @ingroup AH_ExtIO
 */
template <uint8_t N>
class ShiftRegisterOutBase : public StaticSizeExtendedIOElement<N> {
  public:
#if defined(__SAM3X8E__) || defined(__SAMD21G18A__)
    using BitOrder_t = BitOrder;
#else
    using BitOrder_t = uint8_t;
#endif

  protected:
    /**
     * @brief   Create a new ShiftRegisterOutBase object with a given bit order,
     *          and a given number of outputs.
     * 
     * @param   latchPin
     *          The digital output pin connected to the latch pin (ST_CP or 
     *          RCLK) of the shift register.
     * @param   bitOrder
     *          Either `MSBFIRST` (most significant bit first) or `LSBFIRST`
     *          (least significant bit first).
     */
    ShiftRegisterOutBase(pin_t latchPin, BitOrder_t bitOrder);

  public:
    /**
     * @brief   The pinMode function is not implemented because the mode is
     *          `OUTPUT` by definition.
     */
    void pinMode(pin_t pin, uint8_t mode) override __attribute__((deprecated)) {
        (void)pin;
        (void)mode;
    }

    /**
     * @brief   Set the state of a given output pin.
     * 
     * @param   pin
     *          The shift register pin to set.
     * @param   val
     *          The value to set the pin to.
     *          (Either `HIGH` (1) or `LOW` (0))
     */
    void digitalWrite(pin_t pin, uint8_t val) override;

    /**
     * @brief   Get the current state of a given output pin.
     * 
     * @param   pin
     *          The shift register pin to read from.
     * @retval  0
     *          The state of the pin is `LOW`.
     * @retval  1
     *          The state of the pin is `HIGH`.
     */
    int digitalRead(pin_t pin) override;

    /**
     * @brief   The analogRead function is deprecated because a shift
     *          is always digital.
     * @param   pin
     *          The shift register pin to read from.
     * @retval  0
     *          The state of the pin is `LOW`.
     * @retval  1023
     *          The state of the pin is `HIGH`.
     */
    analog_t analogRead(pin_t pin) override __attribute__((deprecated)) {
        return 1023 * digitalRead(pin);
    }

    /**
     * @brief   The analogWrite function is not deprecated because a shift
     *          is always digital.
     * @param   pin
     *          The shift register pin to set.
     * @param   val
     *          The value to set the pin to. A value greater or equal to 0x80
     *          will set the pin to a `HIGH` state, a value less than 0x80 will
     *          set the pin to a `LOW` state.
     */
    void analogWrite(pin_t pin, analog_t val) override
        __attribute__((deprecated)) {
        digitalWrite(pin, val >= 0x80);
    }

    /**
     * @brief   Get the red output pin of the given LED.
     * 
     * @param   id
     *          The zero-based LED number.
     */
    pin_t red(pin_t id);

    /**
     * @brief   Get an array containing all pins with red LEDs.
     */
    Array<pin_t, N / 3> redPins();

    /**
     * @brief   Get the green output pin of the given LED.
     * 
     * @param   id
     *          The zero-based LED number.
     */
    pin_t green(pin_t id);

    /**
     * @brief   Get an array containing all pins with green LEDs.
     */
    Array<pin_t, N / 3> greenPins();

    /**
     * @brief   Get the blue output pin of the given LED.
     * 
     * @param   id
     *          The zero-based LED number.
     */
    pin_t blue(pin_t id);

    /**
     * @brief   Get an array containing all pins with blue LEDs.
     */
    Array<pin_t, N / 3> bluePins();

  protected:
    const pin_t latchPin;
    const BitOrder_t bitOrder;

    BitArray<N> buffer;
    bool dirty = true;
};

END_AH_NAMESPACE

#include "ShiftRegisterOutBase.ipp"

AH_DIAGNOSTIC_POP()
