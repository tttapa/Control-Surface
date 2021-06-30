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
 * 
 * @tparam  NumChips
 *          The number of daisy-chained MAX7219 chips.
 * @tparam  SPIDriver
 *          The SPI class to use. Usually, the default is fine.
 */
template <uint8_t NumChips = 1, class SPIDriver = decltype(SPI) &>
class MAX7219 : public MAX7219_Base<SPIDriver>,
                public StaticSizeExtendedIOElement<8 * 8 * NumChips> {
  public:
    /**
     * @brief   Create a MAX7219 ExtendedIOElement.
     * 
     * @param   spi
     *          The SPI interface to use.
     * @param   loadPin
     *          The pin connected to the load pin (C̄S̄) of the MAX7219.
     */
    MAX7219(SPIDriver spi, pin_t loadPin)
        : MAX7219_Base<SPIDriver>(std::forward<SPIDriver>(spi), loadPin,
                                  NumChips) {}

    /// Initialize.
    /// @see    @ref MAX7219::begin
    void begin() override { MAX7219_Base<SPIDriver>::begin(); }

  private:
    struct IndexMask {
        uint8_t row;
        uint8_t col;
        uint8_t rowgrp;
        uint8_t rowmask;
        uint8_t colmask;
    };

    static IndexMask pin2index(pin_t pin) {
        uint8_t row = pin / 8;
        uint8_t col = pin % 8;
        uint8_t rowgrp = row % 8;
        uint8_t rowmask = 1 << rowgrp;
        uint8_t colmask = 1 << col;
        return {row, col, rowgrp, rowmask, colmask};
    }

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
     *          The output pin to set.
     * @param   val
     *          The value to set the output to.
     *          (Either `HIGH` (1) or `LOW` (0))
     */
    void digitalWrite(pin_t pin, PinStatus_t val) override {
        IndexMask i = pin2index(pin);
        val ? buffer[i.row] |= i.colmask   // set the pin (high)
            : buffer[i.row] &= ~i.colmask; // clear the pin (low)
        updateBufferedOutputRow(i);
    }

    /**
     * @brief   Set the state of a given pin in the software buffer.
     * The buffer is written to the MAX7219 when @ref updateBufferedOutputs
     * is called.
     * @copydetails digitalWrite
     */
    void digitalWriteBuffered(pin_t pin, PinStatus_t val) override {
        IndexMask i = pin2index(pin);
        val ? buffer[i.row] |= i.colmask   // set the pin (high)
            : buffer[i.row] &= ~i.colmask; // clear the pin (low)
        dirty_rows |= i.rowmask;
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
    PinStatus_t digitalRead(pin_t pin) override {
        IndexMask i = pin2index(pin);
        return bool(buffer[i.row] & i.colmask) ? HIGH : LOW;
    }

    /**
     * @copydoc digitalRead
     */
    PinStatus_t digitalReadBuffered(pin_t pin) override {
        IndexMask i = pin2index(pin);
        return bool(buffer[i.row] & i.colmask) ? HIGH : LOW;
    }

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
     * @copydoc analogRead
     */
    analog_t analogReadBuffered(pin_t pin) override
        __attribute__((deprecated)) {
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
        digitalWrite(pin, val >= 0x80 ? HIGH : LOW);
    }

    /**
     * @copydoc analogWrite
     */
    void analogWriteBuffered(pin_t pin, analog_t val) override
        __attribute__((deprecated)) {
        digitalWrite(pin, val >= 0x80 ? HIGH : LOW);
    }

    void updateBufferedOutputRow(IndexMask i) {
        this->sendRowAll(i.rowgrp, buffer.data + i.rowgrp, 8);
        dirty_rows &= ~i.rowmask;
    }

    void updateBufferedOutputs() override {
        if (dirty_rows == 0)
            return;
        uint8_t row = 8;
        do {
            --row;
            if (dirty_rows & 0x80)
                this->sendRowAll(row, buffer.data + row, 8);
            dirty_rows <<= 1;
        } while (row);
    }

    void updateBufferedInputs() override {}

  private:
    Array<uint8_t, 8 * NumChips> buffer;
    uint8_t dirty_rows = 0xFF;
};

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
