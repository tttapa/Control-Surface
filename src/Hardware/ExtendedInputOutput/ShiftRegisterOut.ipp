#include "ExtendedInputOutput.hpp"
#include "ShiftRegisterOut.hpp"

BEGIN_CS_NAMESPACE

template <uint8_t N>
ShiftRegisterOut<N>::ShiftRegisterOut(pin_t dataPin, pin_t clockPin,
                                      pin_t latchPin, uint8_t bitOrder)
    : ShiftRegisterOutBase<N>(latchPin, bitOrder), dataPin(dataPin),
      clockPin(clockPin) {}

template <uint8_t N>
void ShiftRegisterOut<N>::begin() {
    ExtIO::pinMode(dataPin, OUTPUT);
    ExtIO::pinMode(clockPin, OUTPUT);
    ExtIO::pinMode(this->latchPin, OUTPUT);
    update();
}

template <uint8_t N>
void ShiftRegisterOut<N>::update() {
    if (!this->dirty)
        return;
    ExtIO::digitalWrite(this->latchPin, LOW);
    const uint8_t bufferLength = this->buffer.getBufferLength();
    if (this->bitOrder == LSBFIRST)
        for (uint8_t i = 0; i < bufferLength; i++)
            ExtIO::shiftOut(dataPin, clockPin, LSBFIRST,
                            this->buffer.getByte(i));
    else
        for (int8_t i = bufferLength - 1; i >= 0; i--)
            ExtIO::shiftOut(dataPin, clockPin, MSBFIRST,
                            this->buffer.getByte(i));

    ExtIO::digitalWrite(this->latchPin, HIGH);
    this->dirty = false;
}

END_CS_NAMESPACE