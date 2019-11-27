#ifdef ARDUINO // I'm too lazy to mock the SPI library

#include "ExtendedInputOutput.hpp"
#include "SPIShiftRegisterOut.hpp"

AH_DIAGNOSTIC_EXTERNAL_HEADER()
#include <SPI.h>
AH_DIAGNOSTIC_POP()

BEGIN_AH_NAMESPACE

template <uint8_t N>
SPIShiftRegisterOut<N>::SPIShiftRegisterOut(pin_t latchPin, BitOrder_t bitOrder)
    : ShiftRegisterOutBase<N>(latchPin, bitOrder) {}

template <uint8_t N>
void SPIShiftRegisterOut<N>::begin() {
    ExtIO::pinMode(this->latchPin, OUTPUT);
    SPI.begin();
    update();
}

template <uint8_t N>
void SPIShiftRegisterOut<N>::update() {
    if (!this->dirty)
        return;
    SPISettings settings = {SPI_MAX_SPEED, this->bitOrder, SPI_MODE0};
    SPI.beginTransaction(settings);
    ExtIO::digitalWrite(this->latchPin, LOW);
    const uint8_t bufferLength = this->buffer.getBufferLength();
    if (this->bitOrder == LSBFIRST)
        for (uint8_t i = 0; i < bufferLength; i++)
            SPI.transfer(this->buffer.getByte(i));
    else
        for (int8_t i = bufferLength - 1; i >= 0; i--)
            SPI.transfer(this->buffer.getByte(i));
    ExtIO::digitalWrite(this->latchPin, HIGH);
    SPI.endTransaction();
    this->dirty = false;
}

END_AH_NAMESPACE

#endif