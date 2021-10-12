#include "Encoder.h"

BEGIN_CS_NAMESPACE

inline int32_t Encoder::read() {
    if (interrupts_in_use < 2) {
        noInterrupts();
        update(&encoder);
    } else {
        noInterrupts();
    }
    int32_t ret = encoder.position;
    interrupts();
    return ret;
}

inline int32_t Encoder::readAndReset() {
    if (interrupts_in_use < 2) {
        noInterrupts();
        update(&encoder);
    } else {
        noInterrupts();
    }
    int32_t ret = encoder.position;
    encoder.position = 0;
    interrupts();
    return ret;
}

inline void Encoder::write(int32_t p) {
    noInterrupts();
    encoder.position = p;
    interrupts();
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

#if defined(__AVR__)
#include "Encoder-AVR.ipp"
#else
inline void Encoder::update(Encoder_internal_state_t *arg) {
    uint8_t p1val = DIRECT_PIN_READ(arg->pin1_register, arg->pin1_bitmask);
    uint8_t p2val = DIRECT_PIN_READ(arg->pin2_register, arg->pin2_bitmask);
    uint8_t state = arg->state & 3;
    if (p1val)
        state |= 4;
    if (p2val)
        state |= 8;
    arg->state = (state >> 2);
    switch (state) {
        case 1:
        case 7:
        case 8:
        case 14: arg->position++; return;
        case 2:
        case 4:
        case 11:
        case 13: arg->position--; return;
        case 3:
        case 12: arg->position += 2; return;
        case 6:
        case 9: arg->position -= 2; return;
    }
}
#endif

END_CS_NAMESPACE
