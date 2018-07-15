#include "ShiftRegisterOut.h"
#include "ExtendedInputOutput.h"

using namespace ExtIO;

ShiftRegisterOut::ShiftRegisterOut(pin_t dataPin, pin_t clockPin,
                                   pin_t latchPin, uint8_t bitOrder,
                                   pin_t length)
    : ExtendedIOElement(length), dataPin(dataPin), clockPin(clockPin),
      latchPin(latchPin), bitOrder(bitOrder), buffer{(uint8_t)length} {
    begin();
}

void ShiftRegisterOut::digitalWrite(pin_t pin, uint8_t val) {
    buffer.set(pin, val);
    update();
}

int ShiftRegisterOut::digitalRead(pin_t pin) { return buffer.get(pin); }

void ShiftRegisterOut::begin() {
    ExtIO::pinMode(dataPin, OUTPUT);
    ExtIO::pinMode(clockPin, OUTPUT);
    ExtIO::pinMode(latchPin, OUTPUT);
    update();
}

void ShiftRegisterOut::update() {
    ExtIO::digitalWrite(latchPin, LOW);
    const uint8_t bufferLength = buffer.getBufferLength();
    if (bitOrder == LSBFIRST) {
        for (uint8_t i = 0; i < bufferLength; i++)
            shiftOut(dataPin, clockPin, LSBFIRST, buffer.getByte(i));
    } else {
        for (int8_t i = bufferLength - 1; i >= 0; i--)
            ExtIO::shiftOut(dataPin, clockPin, MSBFIRST, buffer.getByte(i));
    }
    ExtIO::digitalWrite(latchPin, HIGH);
}

pin_t ShiftRegisterOut::green(pin_t id) { return pin(3 * id + greenBit); }
pin_t ShiftRegisterOut::red(pin_t id) { return pin(3 * id + redBit); }
pin_t ShiftRegisterOut::blue(pin_t id) { return pin(3 * id + blueBit); }

const uint8_t ShiftRegisterOut::redBit __attribute__((weak)) = 0;
const uint8_t ShiftRegisterOut::greenBit __attribute__((weak)) = 1;
const uint8_t ShiftRegisterOut::blueBit __attribute__((weak)) = 2;