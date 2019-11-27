/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include "StaticSizeExtendedIOElement.hpp"
#include <AH/Containers/BitArray.hpp>
#include <AH/Hardware/LEDs/MAX7219_Base.hpp>

BEGIN_AH_NAMESPACE

/**
 * @brief   A class for LED outputs using the MAX7219 LED display driver. 
 * 
 * The SPI interface is used.
 * 
 * @todo    Wiring diagram for SPI connection.
 */
class MAX7219 : public MAX7219_Base, public StaticSizeExtendedIOElement<8 * 8> {
  public:
    /**
     * @brief   Create a MAX7219 ExtendedIOElement.
     * 
     * @param   loadPin
     *          The pin connected to the load pin (C̄S̄) of the MAX7219.
     */
    MAX7219(pin_t loadPin) : MAX7219_Base(loadPin) {}

    /// Initialize.
    /// @see    MAX7219#init
    void begin() override { init(); }

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
     *          The output pin to set.
     * @param   val
     *          The value to set the output to.
     *          (Either `HIGH` (1) or `LOW` (0))
     */
    void digitalWrite(pin_t pin, uint8_t val) override {
        buffer.set(pin, val);
        update();
    }

    /**
     * @brief   Get the current state of a given output.
     * 
     * @param   pin
     *          The output to read the state from.
     * @retval  0
     *          The state of the output is `LOW`.
     * @retval  1
     *          The state of the output is `HIGH`.
     */
    int digitalRead(pin_t pin) override { return buffer.get(pin); }

    /**
     * @brief   The analogRead function is deprecated because a MAX7219
     *          is always digital.
     * @param   pin
     *          The output to read the state from.
     * @retval  0
     *          The state of the output is `LOW`.
     * @retval  1023
     *          The state of the output is `HIGH`.
     */
    analog_t analogRead(pin_t pin) override __attribute__((deprecated)) {
        return 1023 * digitalRead(pin);
    }

    /**
     * @brief   The analogWrite function is deprecated because a MAX7219
     *          is always digital.
     * @param   pin
     *          The output to set.
     * @param   val
     *          The value to set the output to. A value greater or equal to 0x80
     *          will set the output to a `HIGH` state, a value less than 0x80 
     *          will set the output to a `LOW` state.
     * 
     * @note    This function is just implemented for compatibility. Don't use
     *          it in new code.
     */
    void analogWrite(pin_t pin, analog_t val) override
        __attribute__((deprecated)) {
        digitalWrite(pin, val >= 0x80);
    }

    /**
     * @brief   Write the buffer to the display.
     * 
     * @todo    Does this really have to happen on each update?
     */
    void update() override {
        for (uint8_t i = 0; i < buffer.getBufferLength(); i++)
            sendRaw(i + 1, buffer.getByte(i));
    }

  private:
    BitArray<8 * 8> buffer;
};

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
