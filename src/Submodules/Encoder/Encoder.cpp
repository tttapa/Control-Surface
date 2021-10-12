#ifdef ARDUINO

#include "Encoder.h"

BEGIN_CS_NAMESPACE

#include "codegen/ISRs-def.ipp"

Encoder::Encoder(uint8_t pin1, uint8_t pin2) : pin1(pin1), pin2(pin2) {
    // It's much faster to use the GPIO registers directly, rather than
    // calling digitalRead every time we need to read a pin.
    // digitalRead looks up the register and bitmasks every time it's called
    // but here, we look them up once in the constructor.
    encoder.pin1_register = PIN_TO_BASEREG(pin1);
    encoder.pin1_bitmask = PIN_TO_BITMASK(pin1);
    encoder.pin2_register = PIN_TO_BASEREG(pin2);
    encoder.pin2_bitmask = PIN_TO_BITMASK(pin2);
    encoder.position = 0;
}

Encoder::Encoder(Encoder &&other) { *this = enc_util::move(other); }

Encoder &Encoder::operator=(Encoder &&other) {
    // First swap the normal member variables:
    enc_util::swap(this->pin1, other.pin1);
    enc_util::swap(this->pin2, other.pin2);
    enc_util::swap(this->interrupts_in_use, other.interrupts_in_use);

    // Next, update the pointers in interruptArgs:
    // When interrupts are in use, there is a global interrupt context
    // variable that holds a pointer to the encoders that are being swapped
    // or moved.
    // After moving, this pointer would no longer be valid, so it has to be
    // changed to point to the new encoder object.
    // Calling attachInterrupt is not necessary, because this should already
    // have happened in the begin method if interrupts_in_use is nonzero.
    // Before messing with the state variables that can be changed or
    // accessed from within an ISR, we have to disable interrupts.
    noInterrupts();
    enc_util::swap(this->encoder, other.encoder);
    if (this->interrupts_in_use > 0) {
        auto int1 = digitalPinToInterrupt(this->pin1);
        if (int1 != NOT_AN_INTERRUPT)
            interruptArgs[int1] = &this->encoder;
        auto int2 = digitalPinToInterrupt(this->pin2);
        if (int2 != NOT_AN_INTERRUPT)
            interruptArgs[int2] = &this->encoder;
    }
    if (other.interrupts_in_use > 0) {
        auto int1 = digitalPinToInterrupt(other.pin1);
        if (int1 != NOT_AN_INTERRUPT)
            interruptArgs[int1] = &other.encoder;
        auto int2 = digitalPinToInterrupt(other.pin2);
        if (int2 != NOT_AN_INTERRUPT)
            interruptArgs[int2] = &other.encoder;
    }
    interrupts();

    return *this;
}

Encoder::~Encoder() {
    // If interrupts are in use, there are dangling pointers to the encoder
    // state in the global interrupt contexts. These have to be detached
    // when the encoder object is removed.
    end();
}

void Encoder::begin() {
    pinMode(pin1, INPUT_PULLUP);
    pinMode(pin2, INPUT_PULLUP);
    // allow time for a passive R-C filter to charge
    // through the pullup resistors, before reading
    // the initial state
    delayMicroseconds(2000);
    uint8_t s = 0;
    if (DIRECT_PIN_READ(encoder.pin1_register, encoder.pin1_bitmask))
        s |= 1;
    if (DIRECT_PIN_READ(encoder.pin2_register, encoder.pin2_bitmask))
        s |= 2;
    encoder.state = s;

    attachInterruptCtx(digitalPinToInterrupt(pin1));
    attachInterruptCtx(digitalPinToInterrupt(pin2));
}

void Encoder::end() {
    if (interrupts_in_use > 0) {
        detachInterruptCtx(digitalPinToInterrupt(pin1));
        detachInterruptCtx(digitalPinToInterrupt(pin2));
    }
}

void Encoder::attachInterruptCtx(int interrupt) {
    if (interrupt != NOT_AN_INTERRUPT) {
        interruptArgs[interrupt] = &encoder;
        ++interrupts_in_use;
        attachInterrupt(interrupt, EncoderISRs::getISR(interrupt), CHANGE);
    }
}

void Encoder::detachInterruptCtx(int interrupt) {
    if (interrupt != NOT_AN_INTERRUPT) {
        detachInterrupt(interrupt);
        --interrupts_in_use;
        interruptArgs[interrupt] = nullptr;
    }
}

Encoder_internal_state_t *Encoder::interruptArgs[ENCODER_ARGLIST_SIZE] = {};

END_CS_NAMESPACE

#endif