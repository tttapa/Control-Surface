#ifdef ARDUINO

#include "AHEncoder.hpp"

#include <AH/STL/utility> // std::swap

BEGIN_CS_NAMESPACE

#ifndef CS_CUSTOM_INTERRUPT_TO_INDEX
using interrupt_index_t = decltype(digitalPinToInterrupt(0));
static interrupt_index_t interruptToIndex(interrupt_index_t i) { return i; }
#endif

AHEncoder::AHEncoder(uint8_t pinA, uint8_t pinB)
    : pins {pinA, pinB},
      direct_pins {direct_pin_read(pinA), direct_pin_read(pinB)} {
    // It's much faster to use the GPIO registers directly, rather than
    // calling digitalRead every time we need to read a pin.
    // digitalRead looks up the register and bitmasks every time it's called
    // but here, we look them up once in the constructor.
}

AHEncoder::AHEncoder(AHEncoder &&other)
    : pins (other.pins), direct_pins (std::move(other.direct_pins)) {
    if (other.interrupts_in_use)
        FATAL_ERROR(F("Cannot move from initialized AHEncoder."), 0x9311);
}

AHEncoder &AHEncoder::operator=(AHEncoder &&other) {
    swap(*this, other);
    return *this;
}

void swap(AHEncoder &a, AHEncoder &b) {
    // First swap the normal member variables:
    std::swap(a.interrupts_in_use, b.interrupts_in_use);
    std::swap(a.pins, b.pins);

    // Next, update the pointers in instance_table:
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
    std::swap(a.state, b.state);
    std::swap(a.direct_pins, b.direct_pins);
    std::swap(a.position, b.position);
    if (a.interrupts_in_use > 0) {
        int int1 = digitalPinToInterrupt(a.pins[0]);
        if (int1 != NOT_AN_INTERRUPT)
            AHEncoder::instance_table[interruptToIndex(int1)] = &a;
        int int2 = digitalPinToInterrupt(a.pins[1]);
        if (int2 != NOT_AN_INTERRUPT)
            AHEncoder::instance_table[interruptToIndex(int2)] = &a;
    }
    if (b.interrupts_in_use > 0) {
        int int1 = digitalPinToInterrupt(b.pins[0]);
        if (int1 != NOT_AN_INTERRUPT)
            AHEncoder::instance_table[interruptToIndex(int1)] = &b;
        int int2 = digitalPinToInterrupt(b.pins[1]);
        if (int2 != NOT_AN_INTERRUPT)
            AHEncoder::instance_table[interruptToIndex(int2)] = &b;
    }
    interrupts();
}

AHEncoder::~AHEncoder() {
    // If interrupts are in use, there are dangling pointers to the encoder
    // state in the global interrupt contexts. These have to be detached
    // when the encoder object is removed.
    end();
}

void AHEncoder::begin() {
    if (interrupts_in_use > 0)
        return;
    pinMode(pins[0], INPUT_PULLUP);
    pinMode(pins[1], INPUT_PULLUP);
    // allow time for a passive R-C filter to charge
    // through the pullup resistors, before reading
    // the initial state
    delayMicroseconds(2000);
    uint8_t s = 0;
    if (direct_pins[0].read())
        s |= 1;
    if (direct_pins[1].read())
        s |= 2;
    state = s;

    attachInterruptCtx(digitalPinToInterrupt(pins[0]));
    attachInterruptCtx(digitalPinToInterrupt(pins[1]));
#if defined(ARDUINO_ARCH_MBED) && !defined(ARDUINO_ARCH_RP2040)
    // https://github.com/arduino/ArduinoCore-mbed/issues/253
    pinMode(pins[0], INPUT_PULLUP);
    pinMode(pins[1], INPUT_PULLUP);
#endif
}

void AHEncoder::end() {
    if (interrupts_in_use > 0) {
        detachInterruptCtx(digitalPinToInterrupt(pins[0]));
        detachInterruptCtx(digitalPinToInterrupt(pins[1]));
    }
}

void AHEncoder::attachInterruptCtx(int interrupt) {
    if (interrupt != NOT_AN_INTERRUPT) {
        if (instance_table[interruptToIndex(interrupt)] != nullptr) {
            FATAL_ERROR(F("Multiple encoders on the same pin"), 0x7283);
            return;
        }
        instance_table[interruptToIndex(interrupt)] = this;
        ++interrupts_in_use;
#ifdef ARDUINO_ARCH_RP2040
        gpio_set_irq_enabled_with_callback(
            interrupt, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true,
            [](uint gpio, uint32_t) {
                if (auto arg = instance_table[gpio])
                    arg->update();
            });
#else
        attachInterrupt(interrupt, get_isr(interruptToIndex(interrupt)),
                        CHANGE);
#endif
    }
}

void AHEncoder::detachInterruptCtx(int interrupt) {
    if (interrupt != NOT_AN_INTERRUPT) {
#ifdef ARDUINO_ARCH_RP2040
        gpio_set_irq_enabled(interrupt, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,
                             false);
#else
        detachInterrupt(interrupt);
#endif
        --interrupts_in_use;
        instance_table[interruptToIndex(interrupt)] = nullptr;
    }
}

AHEncoder *AHEncoder::instance_table[CS_ENCODER_ARGLIST_SIZE] {};

END_CS_NAMESPACE

#endif