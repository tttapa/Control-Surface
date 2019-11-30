#include "ShiftRegisterOutRGB.hpp"
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

BEGIN_AH_NAMESPACE

const uint8_t ShiftRegisterOutRGB::redBit __attribute__((weak)) = 0;
const uint8_t ShiftRegisterOutRGB::greenBit __attribute__((weak)) = 1;
const uint8_t ShiftRegisterOutRGB::blueBit __attribute__((weak)) = 2;

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
