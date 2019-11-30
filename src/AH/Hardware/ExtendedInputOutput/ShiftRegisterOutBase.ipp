#include "ExtendedInputOutput.hpp"
#include "ShiftRegisterOutBase.hpp"
#include "ShiftRegisterOutRGB.hpp"

BEGIN_AH_NAMESPACE

template <uint8_t N>
ShiftRegisterOutBase<N>::ShiftRegisterOutBase(pin_t latchPin,
                                              BitOrder_t bitOrder)
    : latchPin(latchPin), bitOrder(bitOrder) {}

template <uint8_t N>
void ShiftRegisterOutBase<N>::digitalWrite(pin_t pin, uint8_t val) {
    buffer.set(pin, val);
    dirty = true;
    this->update(); // TODO: should I always update?
}

template <uint8_t N>
int ShiftRegisterOutBase<N>::digitalRead(pin_t pin) {
    return buffer.get(pin);
}

template <uint8_t N>
pin_t ShiftRegisterOutBase<N>::green(pin_t id) {
    return this->pin(3 * id + ShiftRegisterOutRGB::greenBit);
}

template <uint8_t N>
Array<pin_t, N / 3> ShiftRegisterOutBase<N>::greenPins() {
    return generateIncrementalArray<pin_t, N / 3>(
        this->pin(ShiftRegisterOutRGB::greenBit), 3);
}

template <uint8_t N>
pin_t ShiftRegisterOutBase<N>::red(pin_t id) {
    return this->pin(3 * id + ShiftRegisterOutRGB::redBit);
}

template <uint8_t N>
Array<pin_t, N / 3> ShiftRegisterOutBase<N>::redPins() {
    return generateIncrementalArray<pin_t, N / 3>(
        this->pin(ShiftRegisterOutRGB::redBit), 3);
}

template <uint8_t N>
pin_t ShiftRegisterOutBase<N>::blue(pin_t id) {
    return this->pin(3 * id + ShiftRegisterOutRGB::blueBit);
}

template <uint8_t N>
Array<pin_t, N / 3> ShiftRegisterOutBase<N>::bluePins() {
    return generateIncrementalArray<pin_t, N / 3>(
        this->pin(ShiftRegisterOutRGB::blueBit), 3);
}

END_AH_NAMESPACE