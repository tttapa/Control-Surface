#pragma once

#include "AHEncoder.hpp"

BEGIN_CS_NAMESPACE

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

/*
        // Simple, easy-to-read "documentation" version :-)
        //
        void update(void) {
            uint8_t s = state & 3;
            if (digitalRead(pin1)) s |= 4;
            if (digitalRead(pin2)) s |= 8;
            switch (s) {
                case 0: case 5: case 10: case 15:
                    break;
                case 1: case 7: case 8: case 14:
                    position++; break;
                case 2: case 4: case 11: case 13:
                    position--; break;
                case 3: case 12:
                    position += 2; break;
                default:
                    position -= 2; break;
            }
            state = (s >> 2);
        }
*/

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
