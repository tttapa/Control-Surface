#pragma once

#include "ShiftRegisterOutBase.hpp"
#include <Arduino.h> // MSBFIRST
#include <Helpers/BitArray.hpp>

/**
 * @brief   A class for serial-in/parallel-out shift registers, 
 *          like the 74HC595.
 * 
 * @ingroup ExtIO
 */
class ShiftRegisterOut : public ShiftRegisterOutBase {
  public:
    /**
     * @brief   Create a new ShiftRegisterOut object with a shift register
     *          connected to the given pins, with a given bit order,
     *          and a given number of outputs.
     * 
     * Multiple shift registers can be cascaded by connecting the serial output
     * of the first one to the input of the second one:
     * ```
     * clockPin >───────────┬──────────────────────┬───────── ⋯
     *              ┎━━━━━━━┷━━━━━━━┓      ┎━━━━━━━┷━━━━━━━┓ 
     *              ┃     SH_CP     ┃      ┃     SH_CP     ┃ 
     * dataPin  >───┨ DS        Q7S ┠──────┨ DS        Q7S ┠─ ⋯
     *              ┃     ST_CP     ┃      ┃     ST_CP     ┃ 
     *              ┗━━━━━━━┯━━━━━━━┛      ┗━━━━━━━┯━━━━━━━┛ 
     * latchPin >───────────┴──────────────────────┴───────── ⋯
     * ```
     * 
     * @param   dataPin
     *          The digital output pin connected to the serial data input (DS or
     *          SER) of the shift register.
     * @param   clockPin
     *          The digital output pin connected to the clock input (SH_CP or
     *          SRCLK) of the shift register.
     * @param   latchPin
     *          The digital output pin connected to the latch pin (ST_CP or 
     *          RCLK) of the shift register.
     * @param   bitOrder
     *          Either `MSBFIRST` (most significant bit first) or `LSBFIRST`
     *          (least significant bit first).
     * @param   length
     *          The number of bits in total. Usually, shift registers (e.g. the
     *          74HC595) have eight bits per chip, so `length = 8 * N` where `N`
     *          is the number of cascaded chips.
     */
    ShiftRegisterOut(pin_t dataPin, pin_t clockPin, pin_t latchPin,
                     uint8_t bitOrder = MSBFIRST, pin_t length = 8);

    /**
     * @brief   Initialize the shift register.  
     *          Set the data and clock pins to output mode, 
     *          and set all shift register outputs to `LOW`.
     */
    void begin() override;

    /**
     * @brief   Write the state buffer to the physical outputs.
     */
    void update() override;

  private:
    const pin_t dataPin;
    const pin_t clockPin;
};