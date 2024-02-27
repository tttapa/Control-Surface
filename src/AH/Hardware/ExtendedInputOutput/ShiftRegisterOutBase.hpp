/* ✔ */

#pragma once

#include "ExtendedInputOutput.hpp"
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
template <uint16_t N>
class ShiftRegisterOutBase : public StaticSizeExtendedIOElement<N> {
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
    void pinMode(pin_t pin, PinMode_t mode) override
        __attribute__((deprecated)) {
        (void)pin;
        (void)mode;
    }

    /**
     * @copydoc pinMode
     */
    void pinModeBuffered(pin_t pin, PinMode_t mode) override
        __attribute__((deprecated)) {
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
    void digitalWrite(pin_t pin, PinStatus_t val) override;

    /**
     * @brief   Set the output of a given pin in the software buffer.
     * The buffer is written to the ExtIO device when @ref updateBufferedOutputs
     * is called.
     * @copydetails digitalWrite
     */
    void digitalWriteBuffered(pin_t pin, PinStatus_t val) override;

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
    PinStatus_t digitalRead(pin_t pin) override;

    /** 
     * @copydoc digitalRead
     */
    PinStatus_t digitalReadBuffered(pin_t pin) override {
        return digitalRead(pin);
    }

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
     * @copydoc analogRead
     */
    analog_t analogReadBuffered(pin_t pin) override
        __attribute__((deprecated)) {
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
        digitalWrite(pin, val >= 0x80 ? HIGH : LOW);
    }

    /**
     * @copydoc analogWrite
     */
    void analogWriteBuffered(pin_t pin, analog_t val) override
        __attribute__((deprecated)) {
        digitalWrite(pin, val >= 0x80 ? HIGH : LOW);
    }

    /**
     * @brief   Shift registers don't have an input buffer.
     */
    void updateBufferedInputs() override {} // LCOV_EXCL_LINE

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
