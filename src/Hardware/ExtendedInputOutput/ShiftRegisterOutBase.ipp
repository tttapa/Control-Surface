#include "ExtendedInputOutput.h"
#include "ShiftRegisterOutBase.hpp"
#include "ShiftRegisterOutRGB.hpp"

using namespace ExtIO;

template <uint8_t N>
ShiftRegisterOutBase<N>::ShiftRegisterOutBase(pin_t latchPin, uint8_t bitOrder)
    : ExtendedIOElement(N), latchPin(latchPin), bitOrder(bitOrder) {}

template <uint8_t N>
void ShiftRegisterOutBase<N>::digitalWrite(pin_t pin, uint8_t val) {
    buffer.set(pin, val);
    dirty = true;
    update(); // TODO: should I always update?
}

template <uint8_t N>
int ShiftRegisterOutBase<N>::digitalRead(pin_t pin) {
    return buffer.get(pin);
}

template <uint8_t N>
pin_t ShiftRegisterOutBase<N>::green(pin_t id) {
    return pin(3 * id + ShiftRegisterOutRGB::greenBit);
}
template <uint8_t N>
pin_t ShiftRegisterOutBase<N>::red(pin_t id) {
    return pin(3 * id + ShiftRegisterOutRGB::redBit);
}
template <uint8_t N>
pin_t ShiftRegisterOutBase<N>::blue(pin_t id) {
    return pin(3 * id + ShiftRegisterOutRGB::blueBit);
}