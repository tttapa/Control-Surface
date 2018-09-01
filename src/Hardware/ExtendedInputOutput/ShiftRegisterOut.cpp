#include "ShiftRegisterOut.h"
#include "ExtendedInputOutput.h"

using namespace ExtIO;

ShiftRegisterOut::ShiftRegisterOut(pin_t dataPin, pin_t clockPin,
                                   pin_t latchPin, uint8_t bitOrder,
                                   pin_t length)
    : ShiftRegisterOutBase(latchPin, bitOrder, length), dataPin(dataPin),
      clockPin(clockPin) {}

void ShiftRegisterOut::begin() {
    ExtIO::pinMode(dataPin, OUTPUT);
    ExtIO::pinMode(clockPin, OUTPUT);
    ExtIO::pinMode(latchPin, OUTPUT);
    update();
}

void ShiftRegisterOut::update() {
    if (!dirty)
        return;
    ExtIO::digitalWrite(latchPin, LOW);
    const uint8_t bufferLength = buffer.getBufferLength();
    if (bitOrder == LSBFIRST)
        for (uint8_t i = 0; i < bufferLength; i++)
            ExtIO::shiftOut(dataPin, clockPin, LSBFIRST, buffer.getByte(i));
    else
        for (int8_t i = bufferLength - 1; i >= 0; i--)
            ExtIO::shiftOut(dataPin, clockPin, MSBFIRST, buffer.getByte(i));

    ExtIO::digitalWrite(latchPin, HIGH);
    dirty = false;
}