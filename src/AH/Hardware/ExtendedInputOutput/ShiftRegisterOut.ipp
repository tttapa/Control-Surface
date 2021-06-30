#include "ExtendedInputOutput.hpp"
#include "ShiftRegisterOut.hpp"

BEGIN_AH_NAMESPACE

template <uint16_t N>
ShiftRegisterOut<N>::ShiftRegisterOut(pin_t dataPin, pin_t clockPin,
                                      pin_t latchPin, BitOrder_t bitOrder)
    : ShiftRegisterOutBase<N>(latchPin, bitOrder), dataPin(dataPin),
      clockPin(clockPin) {}

template <uint16_t N>
void ShiftRegisterOut<N>::begin() {
    ExtIO::pinMode(dataPin, OUTPUT);
    ExtIO::pinMode(clockPin, OUTPUT);
    ExtIO::pinMode(this->latchPin, OUTPUT);
    updateBufferedOutputs();
}

template <uint16_t N>
void ShiftRegisterOut<N>::updateBufferedOutputs() {
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

END_AH_NAMESPACE