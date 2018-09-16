#pragma once

#include <Control_Surface.h>
#include <SPI.h>

/*
 * 
 *    __A__
 *   |     |
 * F |     | B
 *   |__G__|
 *   |     |
 * E |     | C
 *   |__D__|
 *  
 */

// Logic Control Specification for seven segment displays
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

static constexpr uint8_t *AlphaCharacters = &SevenSegmentCharacters[0x01];
static constexpr uint8_t *NumericCharacters = &SevenSegmentCharacters[0x30];

class MAX7219SevenSegmentDisplay {

  public:
    MAX7219SevenSegmentDisplay(pin_t loadPin) : loadPin(loadPin) {}

    static constexpr uint8_t DECODEMODE = 9;
    static constexpr uint8_t INTENSITY = 10;
    static constexpr uint8_t SCANLIMIT = 11;
    static constexpr uint8_t SHUTDOWN = 12;
    static constexpr uint8_t DISPLAYTEST = 15;

    void begin() {
      ExtIO::pinMode(loadPin, OUTPUT);
      SPI.begin();
      sendRaw(DISPLAYTEST, 0);        // Normal operation, no test mode
      sendRaw(SCANLIMIT, 7);          // Scan all 8 digits
      sendRaw(DECODEMODE, 0);         // Raw LED addressing
      sendRaw(INTENSITY, 0xF);        // Maximum intensity
      for (uint8_t j = 0; j < 8; j++) // Clear all
        send(j, 0);
      sendRaw(SHUTDOWN, 1); // Enable the display
    }

    void send(uint8_t digit, uint8_t value) {
      sendRaw((digit & 0xF) + 1, value);
    }

    void sendRaw(uint8_t opcode, uint8_t value) {
      ExtIO::digitalWrite(loadPin, LOW);
      SPI.beginTransaction(settings);
      SPI.transfer(opcode);
      SPI.transfer(value);
      ExtIO::digitalWrite(loadPin, HIGH);
      SPI.endTransaction();
    }

    /**
     * @brief   Display a long integer number to the display.
     * 
     * @param   number
     *          The number to display.
     * @param   startDigit
     *          The digit (zero-based, counting from the right) to start 
     *          printing the number.  
     *          Digits: 7 6 5 4 3 2 1 0
     */
    uint8_t display(long number, uint8_t startDigit = 0,
                    uint8_t endDigit = 7) {
      unsigned long start = micros();
      long anumber = abs(number);
      uint8_t i = startDigit + 1;
      endDigit++;
      do {
        sendRaw(i++, NumericCharacters[anumber % 10]);
        anumber /= 10;
      } while (anumber && i <= endDigit);
      if (number < 0 && i <= endDigit)
        sendRaw(i++, 0b00000001); // minus sign
      while (i <= endDigit)
        sendRaw(i++, 0b00000000); // clear unused digits within range
      unsigned long end = micros();
      Serial.println(end - start);
      return i;
    }

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
          d = SevenSegmentCharacters[c - '@'];
        else if (c >= '!' && c <= '?')
          d = SevenSegmentCharacters[c];
        else if (c >= 'a' && c <= 'z')
          d = SevenSegmentCharacters[c - 'a' + 'A' - '@'];
        sendRaw(i--, d);
        prevD = d;
      }
      return 8 - i - startPos;
    }

    template <uint8_t N>
    uint8_t display(const uint8_t (&characters)[N], uint8_t startPos = 0) {
      uint8_t i = 8 - startPos;
      const uint8_t *char_p = &characters[0];
      const uint8_t * const end_p = &characters[N];
      while(i && char_p < end_p) {
        uint8_t c = *char_p++;
        sendRaw(i--, SevenSegmentCharacters[c & 0x3F] | ((c & 0x40) << 1));
      }
      return 8 - i - startPos;
    }

    

  private:
    pin_t loadPin;
    SPISettings settings = {SPI_MAX_SPEED, MSBFIRST, SPI_MODE0};
};