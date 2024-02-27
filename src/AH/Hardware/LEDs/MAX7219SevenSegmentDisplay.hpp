#pragma once

#include "MAX7219_Base.hpp"
#include <AH/STL/cmath> // abs

BEGIN_AH_NAMESPACE

/**
 * @brief   A lookup table for 7-segment characters, as specified by the Logic
 *          Control manual.
 * 
 *       __A__
 *      |     |
 *    F |     | B
 *      |__G__|
 *      |     |
 *    E |     | C
 *      |__D__|    o DP
 */
static constexpr uint8_t SevenSegmentCharacters[0x40] = {
    0b00000000, 0b01110111, 0b00011111, 0b01001110, // @ A B C
    0b00111101, 0b01001111, 0b01000111, 0b01011110, // D E F G
    0b00010111, 0b00110000, 0b00111100, 0b00000111, // H I J K
    0b00001110, 0b01110110, 0b00010101, 0b00011101, // L M N O
    0b01100111, 0b01110011, 0b00000101, 0b01011011, // P Q R S
    0b00001111, 0b00011100, 0b00100111, 0b00111110, // T U V W
    0b00110111, 0b00111011, 0b01101101, 0b01001110, // X Y Z [
    0b00010011, 0b01111000, 0b01100010, 0b00001000, // \ ] ^ _
    0b00000000, 0b00000000, 0b00100010, 0b01100011, //   ! " #
    0b01011011, 0b01100111, 0b01111101, 0b00000010, // $ % & '
    0b01001110, 0b01111000, 0b01100011, 0b00110001, // ( ) * +
    0b00000100, 0b00000001, 0b00001000, 0b00100101, // , - . /
    0b01111110, 0b00110000, 0b01101101, 0b01111001, // 0 1 2 3
    0b00110011, 0b01011011, 0b01011111, 0b01110000, // 4 5 6 7
    0b01111111, 0b01111011, 0b00000000, 0b01000100, // 8 9 : ;
    0b00110001, 0b00001001, 0b00000111, 0b01100101, // < = > ?
};

static constexpr const uint8_t *AlphaChars = &SevenSegmentCharacters[0x01];
static constexpr const uint8_t *NumericChars = &SevenSegmentCharacters[0x30];

/**
 * @brief   A class for 8-digit 7-segment displays with a MAX7219 driver.
 * 
 * @tparam  SPIDriver
 *          The SPI class to use. Usually, the default is fine.
 * 
 * @ingroup AH_HardwareUtils
 */
template <class SPIDriver = decltype(SPI) &>
class MAX7219SevenSegmentDisplay : public MAX7219_Base<SPIDriver> {
  public:
    /**
     * @brief   Create a MAX7219SevenSegmentDisplay.
     * 
     * @param   spi
     *          The SPI interface to use.
     * @param   loadPin
     *          The pin connected to the load pin (C̄S̄) of the MAX7219.
     * @param   chainlength
     *          The number of daisy-chained MAX7219 chips.
     */
    MAX7219SevenSegmentDisplay(SPIDriver spi, pin_t loadPin,
                               uint8_t chainlength = 1)
        : MAX7219_Base<SPIDriver>(std::forward<SPIDriver>(spi), loadPin,
                                  chainlength) {}

    /// Initialize.
    /// @see    @ref MAX7219_Base::begin
    void begin() { MAX7219_Base<SPIDriver>::begin(); }

    /**
     * @brief   Set the value of a single digit.
     * 
     * @param   digit
     *          The digit to set the value of. May be greater than 7 if more 
     *          than one chip are daisy-chained.  
     *          Digit numbering starts from the right, the rightmost digit is
     *          digit zero.
     * @param   value
     *          The value/bit pattern to set the digit to.
     */
    void sendDigit(uint16_t digit, uint8_t value) {
        this->sendRaw((digit % 8) + 1, value, digit / 8);
    }

    /**
     * @brief   Get the total number of digits in this chain of displays, i.e. 
     *          eight times the number of chips.
     */
    uint8_t getNumberOfDigits() const { return 8 * this->getChainLength(); }

    /**
     * @brief   Display a long integer to the display.
     *          The number will be right-aligned.
     * 
     * @param   number
     *          The number to display.
     * @param   startDigit
     *          The digit (zero-based, counting from the right) to start 
     *          printing the number.  
     *          Digits: 7 6 5 4 3 2 1 0
     * @param   endDigit
     *          The last digit (zero-based, counting from the right) that can 
     *          be printed.
     *          If the number is larger than `endDigit - startDigit`, a series
     *          of dashes is displayed. If the number is smaller than 
     *          `endDigit - startDigit`, the leftmost digits including 
     *          `endDigit` are cleared.
     * @return  The number of digits that have been overwritten 
     *          (`endDigit - startDigit`).
     */
    int16_t display(long number, int16_t startDigit = 0,
                    int16_t endDigit = -1) {
        if (startDigit < 0)
            startDigit += getNumberOfDigits();
        if (endDigit < 0)
            endDigit += getNumberOfDigits();
        unsigned long anumber = std::abs(number);
        int16_t i = startDigit;
        do {
            sendDigit(i++, NumericChars[anumber % 10]);
            anumber /= 10;
        } while (anumber && i <= endDigit);
        if (number < 0 && i <= endDigit) {
            sendDigit(i++, 0b00000001); // minus sign
        }
        if (anumber != 0) {
            for (int16_t i = startDigit; i <= endDigit;)
                sendDigit(i++, 0b00000001);
        } else {
            // clear unused digits within range
            while (i <= endDigit)
                sendDigit(i++, 0b00000000);
        }
        return endDigit - startDigit;
    }

    /// @copydoc    display(long, int16_t, int16_t)
    int16_t display(int number, int16_t startDigit = 0, int16_t endDigit = -1) {
        return display(long(number), startDigit, endDigit);
    }

    /**
     * @brief   Display a long unsigned integer to the display.
     *          The number will be right-aligned.
     * 
     * @param   number
     *          The number to display.
     * @param   startDigit
     *          The digit (zero-based, counting from the right) to start 
     *          printing the number.  
     *          Digits: 7 6 5 4 3 2 1 0
     * @param   endDigit
     *          The last digit (zero-based, counting from the right) that can 
     *          be printed.
     *          If the number is larger than `endDigit - startDigit`, a series
     *          of dashes is displayed. If the number is smaller than 
     *          `endDigit - startDigit`, the leftmost digits including 
     *          `endDigit` are cleared.
     * @return  The number of digits that have been overwritten 
     *          (`endDigit - startDigit`).
     */
    int16_t display(unsigned long number, int16_t startDigit = 0,
                    int16_t endDigit = -1) {
        if (startDigit < 0)
            startDigit += getNumberOfDigits();
        if (endDigit < 0)
            endDigit += getNumberOfDigits();
        int16_t i = startDigit;
        do {
            sendDigit(i++, NumericChars[number % 10]);
            number /= 10;
        } while (number && i <= endDigit);
        if (number != 0) {
            for (int16_t i = startDigit; i <= endDigit;)
                sendDigit(i++, 0b00000001);
        } else {
            // clear unused digits within range
            while (i <= endDigit)
                sendDigit(i++, 0b00000000);
        }
        return endDigit - startDigit;
    }

    /// @copydoc    display(unsigned long, int16_t, int16_t)
    int16_t display(unsigned int number, int16_t startDigit = 0,
                    int16_t endDigit = -1) {
        return display((unsigned long)(number), startDigit, endDigit);
    }

    /**
     * @brief   Display a string of text to the display.
     * 
     * Full stops are printed to the decimal point between characters.
     * 
     * @param   text 
     *          The null-terminated string to display.
     * @param   startPos
     *          The position to start printing.
     */
    int16_t display(const char *text, int16_t startPos = 0) {
        if (startPos < 0)
            startPos += getNumberOfDigits();
        int16_t i = getNumberOfDigits() - startPos;
        char prevD = '\0';
        while (*text && (i > 0 || *text == '.')) {
            char c = *text++;
            uint8_t d = 0;
            if (c == '.') {
                if (prevD) {
                    sendDigit(i, prevD | 0b10000000);
                    prevD = '\0';
                    continue;
                } else {
                    sendDigit(--i, 0b10000000);
                    continue;
                }
            } else if (c >= '@' && c <= '_')
                d = SevenSegmentCharacters[(uint8_t)c - '@'];
            else if (c >= '!' && c <= '?')
                d = SevenSegmentCharacters[(uint8_t)c];
            else if (c >= 'a' && c <= 'z')
                d = SevenSegmentCharacters[(uint8_t)c - 'a' + 'A' - '@'];
            sendDigit(--i, d);
            prevD = d;
        }
        return getNumberOfDigits() - i - startPos;
    }

    /**
     * @brief   Print a single hexadecimal digit.
     * 
     * @param   digit
     *          The digit to print to [0, 7].
     * @param   value
     *          The 4-bit value to print [0, 15].
     */
    void printHexChar(int16_t digit, uint8_t value) {
        if (digit < 0)
            digit += getNumberOfDigits();
        value &= 0x0F;
        uint8_t c = value >= 0x0A //
                        ? AlphaChars[value - 0x0A]
                        : NumericChars[value];
        sendDigit(digit, c);
    }

    /**
     * @brief   Print a number to the display in hexadecimal format.
     * @param   number
     *          The number to display.
     * @param   startDigit
     *          The digit (zero-based, counting from the right) to start 
     *          printing the number.  
     *          Digits: 7 6 5 4 3 2 1 0
     * @param   endDigit
     *          The last digit (zero-based, counting from the right) that can 
     *          be printed.
     *          If the number is larger than `endDigit - startDigit`, a series
     *          of dashes is displayed. If the number is smaller than 
     *          `endDigit - startDigit`, the leftmost digits including 
     *          `endDigit` are cleared.
     * @return  The number of digits that have been overwritten 
     *          (`endDigit - startDigit`).
     */
    int16_t displayHex(unsigned long number, int16_t startDigit = 0,
                       int16_t endDigit = -1) {
        if (startDigit < 0)
            startDigit += getNumberOfDigits();
        if (endDigit < 0)
            endDigit += getNumberOfDigits();
        int16_t i = startDigit;
        do {
            printHexChar(i++, uint8_t(number));
            number >>= 4;
        } while (number && i <= endDigit);
        if (number != 0) {
            for (int16_t i = startDigit; i <= endDigit;)
                sendDigit(i++, 0b00000001);
        } else {
            // clear unused digits within range
            while (i <= endDigit)
                sendDigit(i++, 0b00000000);
        }
        return endDigit - startDigit;
    }
};

END_AH_NAMESPACE
