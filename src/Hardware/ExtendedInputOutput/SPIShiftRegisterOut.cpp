#ifdef ARDUINO // I'm too lazy to mock the SPI library

#include "SPIShiftRegisterOut.hpp"
#include "ExtendedInputOutput.h"
#include <SPI.h>

using namespace ExtIO;

SPIShiftRegisterOut::SPIShiftRegisterOut(pin_t latchPin, uint8_t bitOrder,
                                         pin_t length)
    : ShiftRegisterOutBase(latchPin, bitOrder, length) {}

void SPIShiftRegisterOut::begin() {
    ExtIO::pinMode(latchPin, OUTPUT);
    SPI.begin();
    update();
}

void SPIShiftRegisterOut::update() {
    if (!dirty)
        return;
    SPISettings settings = {SPI_MAX_SPEED, bitOrder, SPI_MODE0};
    SPI.beginTransaction(settings);
    ExtIO::digitalWrite(latchPin, LOW);
    const uint8_t bufferLength = buffer.getBufferLength();
    if (bitOrder == LSBFIRST)
        for (uint8_t i = 0; i < bufferLength; i++)
            SPI.transfer(buffer.getByte(i));
    else
        for (int8_t i = bufferLength - 1; i >= 0; i--)
            SPI.transfer(buffer.getByte(i));
    ExtIO::digitalWrite(latchPin, HIGH);
    SPI.endTransaction();
    dirty = false;
}

#endif