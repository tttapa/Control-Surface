#pragma once

/// Timer 2 clock select (Table 17-9).
enum class Timer2Prescaler : uint8_t {
    None = 0b000,
    S1 = 0b001,
    S8 = 0b010,
    S32 = 0b011,
    S64 = 0b100,
    S128 = 0b101,
    S256 = 0b110,
    S1024 = 0b111,
    Invalid = 0xFF,
};

/// Convert the prescaler factor to the correct bit pattern to write to the
/// TCCR0B register (Table 17-9).
constexpr inline Timer2Prescaler factorToTimer2Prescaler(uint16_t factor) {
    return factor == 1      ? Timer2Prescaler::S1
           : factor == 8    ? Timer2Prescaler::S8
           : factor == 32   ? Timer2Prescaler::S32
           : factor == 64   ? Timer2Prescaler::S64
           : factor == 128  ? Timer2Prescaler::S128
           : factor == 256  ? Timer2Prescaler::S256
           : factor == 1024 ? Timer2Prescaler::S1024
                            : Timer2Prescaler::Invalid;
}

/// Timer 2 waveform generation mode (Table 17-8).
enum class Timer2WGMode : uint8_t {
    Normal = 0b000,
    PWM = 0b001,
    CTC = 0b010,
    FastPWM = 0b011,
    PWM_OCRA = 0b101,
    FastPWM_OCRA = 0b111,
};

/// Set the clock source/prescaler of Timer2 (Table 17-9).
inline void setTimer2Prescaler(Timer2Prescaler ps) {
    if (ps == Timer2Prescaler::Invalid)
        return;
    bitWrite(TCCR2B, CS22, (static_cast<uint8_t>(ps) >> 2) & 1);
    bitWrite(TCCR2B, CS21, (static_cast<uint8_t>(ps) >> 1) & 1);
    bitWrite(TCCR2B, CS20, (static_cast<uint8_t>(ps) >> 0) & 1);
}

/// Set the wavefrom generation mode of Timer2 (Table 17-8).
inline void setTimer2WGMode(Timer2WGMode mode) {
    bitWrite(TCCR2B, WGM22, (static_cast<uint8_t>(mode) >> 2) & 1);
    bitWrite(TCCR2A, WGM21, (static_cast<uint8_t>(mode) >> 1) & 1);
    bitWrite(TCCR2A, WGM20, (static_cast<uint8_t>(mode) >> 0) & 1);
}
