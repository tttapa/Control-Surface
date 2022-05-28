#pragma once

#include "AHEncoder.hpp"

BEGIN_CS_NAMESPACE

template <unsigned NumISR>
auto AHEncoder::get_isr(unsigned interrupt) -> isr_func_t {
    return interrupt == NumISR - 1
               ? []() CS_ENCODER_ISR_ATTR { AHEncoder::instance_table[NumISR - 1]->update(); }
               : get_isr<NumISR - 1>(interrupt); // Compile-time tail recursion
}

template <>
inline auto AHEncoder::get_isr<0>(unsigned) -> isr_func_t {
    return nullptr;
}

inline int32_t AHEncoder::read() {
    if (interrupts_in_use < 2)
        update();
    return position.get();
}

inline int32_t AHEncoder::readAndReset(int32_t newpos) {
    if (interrupts_in_use < 2)
        update();
    return position.xchg(newpos);
}

inline void AHEncoder::write(int32_t p) {
    if (interrupts_in_use < 2)
        update();
    position.set(p);
}

//                           _______         _______
//               Pin1 ______|       |_______|       |______ Pin1
// negative <---         _______         _______         __      --> positive
//               Pin2 __|       |_______|       |_______|   Pin2

//        new      new      old      old
//        pin2     pin1     pin2     pin1     Result
//        ----     ----     ----     ----     ------
//        0        0        0        0        no movement
//        0        0        0        1        +1
//        0        0        1        0        -1
//        0        0        1        1        +2  (assume pin1 edges only)
//        0        1        0        0        -1
//        0        1        0        1        no movement
//        0        1        1        0        -2  (assume pin1 edges only)
//        0        1        1        1        +1
//        1        0        0        0        +1
//        1        0        0        1        -2  (assume pin1 edges only)
//        1        0        1        0        no movement
//        1        0        1        1        -1
//        1        1        0        0        +2  (assume pin1 edges only)
//        1        1        0        1        -1
//        1        1        1        0        +1
//        1        1        1        1        no movement

inline void AHEncoder::update() {
    uint8_t s = state & 0b11;
    if (direct_pins[0].read())
        s |= 4;
    if (direct_pins[1].read())
        s |= 8;
    state = (s >> 2);
    switch (s) {
        case 1:
        case 7:
        case 8:
        case 14: position.add_isr(1); return;
        case 2:
        case 4:
        case 11:
        case 13: position.add_isr(-1); return;
        case 3:
        case 12: position.add_isr(2); return;
        case 6:
        case 9: position.add_isr(-2); return;
    }
}

END_CS_NAMESPACE
