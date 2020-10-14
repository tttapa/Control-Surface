#pragma once

#include <AH/STL/utility> // std::forward
#include <AH/STL/vector>  // std::vector
#include <Settings/NamespaceSettings.hpp>
#include <ctype.h> // isxdigit, tolower

BEGIN_CS_NAMESPACE

/**
 * @brief   Class that parses hexadecimal ASCII text to bytes.
 *
 * For example, parses the string `"7f A 123456"` to the bytes 
 * `{0x7F, 0x0A, 0x12, 0x34, 0x56}`.
 *
 * @tparam  CharPuller
 *          Class that supplies the ASCII characters to parse.
 *
 * @see     @ref BufferPuller
 * @see     @ref StreamPuller
 * 
 * @ingroup MIDIParsers
 */
template <class CharPuller>
class HexPuller {
  public:
    HexPuller(CharPuller &&puller) : puller(std::forward<CharPuller>(puller)) {}

    /// Pull out a new byte. Pulls characters from the `CharPuller` until a 
    /// hexadecimal number was found, decodes it, and returns it.
    ///
    /// @param[out] output
    ///             A new byte (if available).
    /// @return True if a byte was available, false otherwise.
    bool pull(uint8_t &output) {
        uint8_t input;
        while (puller.pull(input)) {
            // If we receive a hexadecimal digit
            if (isxdigit(input)) {
                (char1 ? char2 : char1) = tolower(input);
            }
            // If we received two hex characters
            if (char1 && char2) {
                output = hex2int(char1) << 4 | hex2int(char2);
                char1 = '\0';
                char2 = '\0';
                return true;
            }
            // If we received one hex character followed by whitespace/other
            else if (!isxdigit(input) && char1) {
                output = hex2int(char1);
                char1 = '\0';
                return true;
            }
        }
        return false;
    }

  private:
    /// Convert a hexadecimal character to a 4-bit nibble.
    static uint8_t hex2int(char hex) {
        return hex < 'a' ? hex - '0' : hex - 'a' + 10;
    }

  public:
    CharPuller puller;

  private:
    char char1 = '\0';
    char char2 = '\0';
};

END_CS_NAMESPACE