#include "ShiftRegisterOutBase.hpp"
#include "ExtendedInputOutput.h"

using namespace ExtIO;

ShiftRegisterOutBase::ShiftRegisterOutBase(pin_t latchPin, uint8_t bitOrder,
                                           pin_t length)
    : ExtendedIOElement(length), latchPin(latchPin),
      bitOrder(bitOrder), buffer{(uint8_t)length} {}

void ShiftRegisterOutBase::digitalWrite(pin_t pin, uint8_t val) {
    buffer.set(pin, val);
    dirty = true;
    update(); // TODO: should I always update?
}

int ShiftRegisterOutBase::digitalRead(pin_t pin) { return buffer.get(pin); }

pin_t ShiftRegisterOutBase::green(pin_t id) { return pin(3 * id + greenBit); }
pin_t ShiftRegisterOutBase::red(pin_t id) { return pin(3 * id + redBit); }
pin_t ShiftRegisterOutBase::blue(pin_t id) { return pin(3 * id + blueBit); }

const uint8_t ShiftRegisterOutBase::redBit __attribute__((weak)) = 0;
const uint8_t ShiftRegisterOutBase::greenBit __attribute__((weak)) = 1;
const uint8_t ShiftRegisterOutBase::blueBit __attribute__((weak)) = 2;