#pragma once

#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.h>
#include <SPI.h>

class MAX7219_Base {

  public:
    MAX7219_Base(pin_t loadPin) : loadPin(loadPin) {}

    static constexpr uint8_t DECODEMODE = 9;
    static constexpr uint8_t INTENSITY = 10;
    static constexpr uint8_t SCANLIMIT = 11;
    static constexpr uint8_t SHUTDOWN = 12;
    static constexpr uint8_t DISPLAYTEST = 15;

    void init() {
        ExtIO::digitalWrite(loadPin, HIGH);
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

    void setIntensity(uint8_t intensity) {
        sendRaw(INTENSITY, intensity & 0xF);
    }

  private:
    pin_t loadPin;
    SPISettings settings = {SPI_MAX_SPEED, MSBFIRST, SPI_MODE0};
};