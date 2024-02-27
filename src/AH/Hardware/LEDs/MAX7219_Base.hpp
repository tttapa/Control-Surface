/* ✔ */

#pragma once

#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>

AH_DIAGNOSTIC_EXTERNAL_HEADER()
#include <SPI.h>
AH_DIAGNOSTIC_POP()

BEGIN_AH_NAMESPACE

/**
 * @brief   A base class for classes that control MAX7219 LED drivers.
 * 
 * @tparam  SPIDriver
 *          The SPI class to use. Usually, the default is fine.
 * 
 * @todo    Wiring diagram for SPI connection.
 */
template <class SPIDriver = decltype(SPI) &>
class MAX7219_Base {
  public:
    /**
     * @brief   Create a MAX7219_Base object.
     * 
     * @param   spi
     *          The SPI interface to use.
     * @param   loadPin
     *          The pin connected to the load pin (C̄S̄) of the MAX7219.
     * @param   chainlength
     *          The number of daisy-chained MAX7219 chips.
     */
    MAX7219_Base(SPIDriver spi, pin_t loadPin, uint8_t chainlength = 1)
        : spi(std::forward<SPIDriver>(spi)), loadPin(loadPin),
          chainlength(chainlength) {}

    static constexpr uint8_t DECODEMODE = 9;
    static constexpr uint8_t INTENSITY = 10;
    static constexpr uint8_t SCANLIMIT = 11;
    static constexpr uint8_t SHUTDOWN = 12;
    static constexpr uint8_t DISPLAYTEST = 15;

    /**
     * @brief   Initialize the Arduino pins, SPI, and the MAX7219.
     */
    void begin() {
        ExtIO::digitalWrite(loadPin, HIGH);
        ExtIO::pinMode(loadPin, OUTPUT);
        spi.begin();
        sendRawAll(DISPLAYTEST, 0); // Normal operation, no test mode
        sendRawAll(SCANLIMIT, 7);   // Scan all 8 digits
        sendRawAll(DECODEMODE, 0);  // Raw LED addressing
        sendRawAll(INTENSITY, 0xF); // Maximum intensity
        clear();
        sendRawAll(SHUTDOWN, 1); // Enable the display
    }

    /** 
     * @brief   Turn off all LEDs.
     */
    void clear() {
        for (uint8_t j = 1; j < 8 + 1; j++)
            sendRawAll(j, 0);
    }

    /**
     * @brief   Send the value to the given digit or row.
     * 
     * @param   digit
     *          The digit or row to set [0, 7].
     * @param   value
     *          The value to set the row to.
     * @param   chip
     *          The chip to send the digit to.
     */
    void send(uint8_t digit, uint8_t value, uint8_t chip = 0) {
        sendRaw((digit & 0x7) + 1, value, chip);
    }

    /**
     * @brief   Send values to the given digit or row, sending a different value
     *          for each chip.
     * 
     * The array is often a matrix containing the values for all rows of all 
     * chips in this chain. In that case, the leading dimension is 8, because 
     * each chip has 8 rows.
     * 
     *     [0] chip 0, row 0  ┐
     *     [1] chip 0, row 1  │
     *     [ ] ...            │ 8 elements between two chips
     *     [7] chip 0, row 7  │
     *     [8] chip 1, row 0  ┘
     *     [9] chip 1, row 1
     *     [ ] ...
     * 
     * If you just want to specify a single row for all chips, the leading 
     * dimension of the array is 1.
     * 
     *     [0] chip 0, row 0  ┐
     *     [1] chip 1, row 0  ┘ 1 element between two chips
     *     [2] chip 2, row 0
     *     [ ] ...
     * 
     * @param   digit
     *          The digit or row to set [0, 7].
     * @param   values
     *          The array of values to send.
     * @param   leading_dim
     *          The leading dimension of the array of values.
     */
    void sendRowAll(uint8_t digit, const uint8_t *values,
                    uint8_t leading_dim = 1) {
        uint8_t opcode = (digit & 0x7) + 1;
        ExtIO::digitalWrite(loadPin, LOW);
        spi.beginTransaction(settings);
        for (uint8_t i = 0; i < chainlength; ++i) {
            spi.transfer(opcode);
            spi.transfer(values[uint16_t(i) * leading_dim]);
        }
        ExtIO::digitalWrite(loadPin, HIGH);
        spi.endTransaction();
    }

    /**
     * @brief   Send different values to all digits/rows of all chips.
     *
     * The array layout should be as follows: 
     *
     *     [0] chip 0, row 0  ┐
     *     [1] chip 0, row 1  │
     *     [ ] ...            │ 8 elements between two chips
     *     [7] chip 0, row 7  │
     *     [8] chip 1, row 0  ┘
     *     [9] chip 1, row 1
     *     [ ] ...
     * 
     * The array has 8 * `chainlength` elements in total.
     * 
     * @param   values
     *          The array of values to send.
     */
    void sendAll(const uint8_t *values) {
        for (uint8_t row = 0; row < 8; ++row)
            sendRowAll(row, values + row, 8);
    }

    /**
     * @brief   Send the same raw opcode and value to all chips in the chain.
     * 
     * @param   opcode
     *          The opcode to send.
     * @param   value
     *          The value to send.
     */
    void sendRawAll(uint8_t opcode, uint8_t value) {
        ExtIO::digitalWrite(loadPin, LOW);
        spi.beginTransaction(settings);
        for (uint8_t i = 0; i < chainlength; ++i) {
            spi.transfer(opcode);
            spi.transfer(value);
        }
        ExtIO::digitalWrite(loadPin, HIGH);
        spi.endTransaction();
    }

    /**
     * @brief   Send a raw opcode and value to the given MAX7219.
     * 
     * @param   opcode
     *          The opcode to send.
     * @param   value
     *          The value to send.
     * @param   chip
     *          The chip to send the command to.
     */
    void sendRaw(uint8_t opcode, uint8_t value, uint8_t chip = 0) {
        if (chip >= chainlength)
            return; // Should I throw an error?
        ExtIO::digitalWrite(loadPin, LOW);
        spi.beginTransaction(settings);
        uint8_t c = 0;
        for (; c < chip; c++) {
            spi.transfer(0x00); // No-Op
            spi.transfer(0x00);
        }
        spi.transfer(opcode);
        spi.transfer(value);
        for (c++; c < chainlength; c++) {
            spi.transfer(0x00); // No-Op
            spi.transfer(0x00);
        }
        ExtIO::digitalWrite(loadPin, HIGH);
        spi.endTransaction();
    }

    /**
     * @brief   Set the intensity of the LEDs of all chips.
     * 
     * @param   intensity
     *          The intensity [0, 15].
     */
    void setIntensity(uint8_t intensity) {
        sendRawAll(INTENSITY, intensity & 0xF);
    }

    /**
     * @brief   Set the intensity of the LEDs of a specific chip.
     * 
     * @param   intensity
     *          The intensity [0, 15].
     * @param   chip
     *          The chip to set the intensity of.
     */
    void setIntensity(uint8_t intensity, uint8_t chip) {
        sendRaw(INTENSITY, intensity & 0xF, chip);
    }

    /**
     * @brief   Get the number of daisy-chained chips.
     */
    uint8_t getChainLength() const { return chainlength; }

  private:
    SPIDriver spi;
    pin_t loadPin;
    uint8_t chainlength;

  public:
    SPISettings settings{SPI_MAX_SPEED, MSBFIRST, SPI_MODE0};
};

END_AH_NAMESPACE
