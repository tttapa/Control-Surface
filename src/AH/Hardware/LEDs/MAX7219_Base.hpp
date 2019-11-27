/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>

AH_DIAGNOSTIC_EXTERNAL_HEADER()
#include <SPI.h>
AH_DIAGNOSTIC_POP()

BEGIN_AH_NAMESPACE

/**
 * @brief   A base class for classes that control MAX7219 LED drivers.
 * 
 * The SPI interface is used.
 * 
 * @todo    Wiring diagram for SPI connection.
 */
class MAX7219_Base {
  public:
    /**
     * @brief   Create a MAX7219_Base object.
     * 
     * @param   loadPin
     *          The pin connected to the load pin (C̄S̄) of the MAX7219.
     */
    MAX7219_Base(pin_t loadPin) : loadPin(loadPin) {}

    static constexpr uint8_t DECODEMODE = 9;
    static constexpr uint8_t INTENSITY = 10;
    static constexpr uint8_t SCANLIMIT = 11;
    static constexpr uint8_t SHUTDOWN = 12;
    static constexpr uint8_t DISPLAYTEST = 15;

    /**
     * @brief   Initialize the Arduino pins, SPI, and the MAX7219.
     * @todo    Rename to `update`.
     */
    void init() {
        ExtIO::digitalWrite(loadPin, HIGH);
        ExtIO::pinMode(loadPin, OUTPUT);
        SPI.begin();
        sendRaw(DISPLAYTEST, 0);        // Normal operation, no test mode
        sendRaw(SCANLIMIT, 7);          // Scan all 8 digits
        sendRaw(DECODEMODE, 0);         // Raw LED addressing
        sendRaw(INTENSITY, 0xF);        // Maximum intensity
        clear();
        sendRaw(SHUTDOWN, 1); // Enable the display
    }

    /** 
     * @brief   Turn off all LEDs.
     */
    void clear() {
        for (uint8_t j = 1; j < 8 + 1; j++)
            sendRaw(j, 0);
    }

    /**
     * @brief   Send the value to the given digit.
     * 
     * @param   digit
     *          The digit or row to set [0, 7].
     * @param   value
     *          The value to set the row to.
     */
    void send(uint8_t digit, uint8_t value) {
        sendRaw((digit & 0x7) + 1, value);
    }

    /**
     * @brief   Send a raw opcode and value to the MAX7219.
     * 
     * @param   opcode
     *          The opcode to send.
     * @param   value
     *          The value to send.
     */
    void sendRaw(uint8_t opcode, uint8_t value) {
        ExtIO::digitalWrite(loadPin, LOW);
        SPI.beginTransaction(settings);
        SPI.transfer(opcode);
        SPI.transfer(value);
        ExtIO::digitalWrite(loadPin, HIGH);
        SPI.endTransaction();
    }

    /**
     * @brief   Set the intensity of the LEDs.
     * 
     * @param   intensity
     *          The intensity [0, 15].
     */
    void setIntensity(uint8_t intensity) {
        sendRaw(INTENSITY, intensity & 0xF);
    }

  private:
    pin_t loadPin;
    SPISettings settings = {SPI_MAX_SPEED, MSBFIRST, SPI_MODE0};
};

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
