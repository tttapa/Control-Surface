#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include "MAX7219_Base.hpp"

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
 * @ingroup AH_HardwareUtils
 */
class MAX7219SevenSegmentDisplay : public MAX7219_Base {
  public:
    /**
     * @brief   Create a MAX7219SevenSegmentDisplay.
     * 
     * @param   loadPin
     *          The pin connected to the load pin (C̄S̄) of the MAX7219.
     */
    MAX7219SevenSegmentDisplay(pin_t loadPin) : MAX7219_Base(loadPin) {}

    /// Initialize.
    /// @see    MAX7219#init
    void begin() { init(); }

    /**
     * @brief   Display a long integer number to the display.
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
     *          If the number is larger than `endDigit - startDigit`, the number
     *          is truncated on the left. If the number is smaller than 
     *          `endDigit - startDigit`, the leftmost digits including 
     *          `endDigit` are cleared.
     * @return  The number of digits that have been overwritten 
     *          (`endDigit - startDigit`).
     */
    uint8_t display(long number, uint8_t startDigit = 0, uint8_t endDigit = 7) {
        long anumber = abs(number);
        uint8_t i = startDigit + 1;
        endDigit++;
        do {
            sendRaw(i++, NumericChars[anumber % 10]);
            anumber /= 10;
        } while (anumber && i <= endDigit);
        if (number < 0 && i <= endDigit)
            sendRaw(i++, 0b00000001); // minus sign
        while (i <= endDigit)
            sendRaw(i++, 0b00000000); // clear unused digits within range
        return endDigit - startDigit;
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
    uint8_t display(const char *text, uint8_t startPos = 0) {
        uint8_t i = 8 - startPos;
        char prevD = '\0';
        while (*text && (i > 0 || *text == '.')) {
            char c = *text++;
            uint8_t d = 0;
            if (c == '.') {
                if (prevD) {
                    sendRaw(1 + i, prevD | 0b10000000);
                    prevD = '\0';
                    continue;
                } else {
                    sendRaw(i--, 0b10000000);
                    continue;
                }
            } else if (c >= '@' && c <= '_')
                d = SevenSegmentCharacters[(uint8_t)c - '@'];
            else if (c >= '!' && c <= '?')
                d = SevenSegmentCharacters[(uint8_t)c];
            else if (c >= 'a' && c <= 'z')
                d = SevenSegmentCharacters[(uint8_t)c - 'a' + 'A' - '@'];
            sendRaw(i--, d);
            prevD = d;
        }
        return 8 - i - startPos;
    }

    /**
     * @brief   ?
     * 
     * @todo    Find out what this function does, and write documentation.
     */
    template <uint8_t N>
    uint8_t display(const uint8_t (&characters)[N], uint8_t startPos = 0) {
        uint8_t i = 8 - startPos;
        const uint8_t *char_p = &characters[0];
        const uint8_t *const end_p = &characters[N];
        while (i && char_p < end_p) {
            uint8_t c = *char_p++;
            sendRaw(i--, SevenSegmentCharacters[(uint8_t)c & 0x3F] |
                             (((uint8_t)c & 0x40) << 1));
        }
        return 8 - i - startPos;
    }

    /**
     * @brief   Print a single hexadecimal digit.
     * 
     * @param   digit
     *          The digit to print to [0, 7].
     * @param   value
     *          The 4-bit value to print [0, 15].
     * 
     * @todo    Rename to `printHexChar` and create function that actually 
     *          prints longer hexadecimal numbers.
     */
    void printHex(uint8_t digit, uint8_t value) {
        value &= 0x0F;
        uint8_t c =
            value >= 0xA ? AlphaChars[value - 0x0A] : NumericChars[value];
        send(digit, c);
    }
};

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
