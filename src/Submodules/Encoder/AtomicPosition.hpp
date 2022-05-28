#pragma once

#include <AH/Arduino-Wrapper.h>
#include <Settings/NamespaceSettings.hpp>

#ifdef __AVR__
#include <util/atomic.h>
#else
#include <atomic>
#endif
#ifdef ARDUINO_ARCH_MBED
#include <CriticalSectionLock.h>
#endif

BEGIN_CS_NAMESPACE

#if defined(ATOMIC_INT_LOCK_FREE) && ATOMIC_INT_LOCK_FREE == 2
template <class T>
struct AtomicPosition {
    using type = T;
    std::atomic<type> value {};

    AtomicPosition(T t) : value {t} {}
    AtomicPosition(const AtomicPosition &o) : AtomicPosition {o.get()} {}
    AtomicPosition(AtomicPosition &&o) : AtomicPosition {o.get()} {}
    AtomicPosition &operator=(const AtomicPosition &o) {
        this->set(o.get());
        return *this;
    }
    AtomicPosition &operator=(AtomicPosition &&o) {
        this->set(o.get());
        return *this;
    }

    constexpr static std::memory_order mo_rlx = std::memory_order_relaxed;
    void add(type other) { value.fetch_add(other, mo_rlx); }
    type get() const { return value.load(mo_rlx); }
    void set(type other) { value.store(other, mo_rlx); }
    type xchg(type other) { return value.exchange(other, mo_rlx); }
    void add_isr(type other) { add(other); }
};
#elif defined(__AVR__)
template <class T>
struct AtomicPosition {
    using type = T;
    volatile type value;

    AtomicPosition(T t) : value {t} {}
    AtomicPosition(const AtomicPosition &o) : AtomicPosition {o.get()} {}
    AtomicPosition(AtomicPosition &&o) : AtomicPosition {o.get()} {}
    AtomicPosition &operator=(const AtomicPosition &o) {
        this->set(o.get());
        return *this;
    }
    AtomicPosition &operator=(AtomicPosition &&o) {
        this->set(o.get());
        return *this;
    }

    void add(type other) {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { value += other; }
    }
    type get() const {
        type copy;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { copy = value; }
        return copy;
    }
    void set(type other) {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { value = other; }
    }
    type xchg(type other) {
        type copy;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            copy = value;
            value = other;
        }
        return copy;
    }
    void add_isr(type other) { value += other; }
};
#elif defined(ARDUINO_ARCH_MBED)
template <class T>
struct AtomicPosition {
    using type = T;
    volatile type value;

    AtomicPosition(T t) : value {t} {}
    AtomicPosition(const AtomicPosition &o) : AtomicPosition {o.get()} {}
    AtomicPosition(AtomicPosition &&o) : AtomicPosition {o.get()} {}
    AtomicPosition &operator=(const AtomicPosition &o) {
        this->set(o.get());
        return *this;
    }
    AtomicPosition &operator=(AtomicPosition &&o) {
        this->set(o.get());
        return *this;
    }

    void add(type other) {
        mbed::CriticalSectionLock lck;
        value += other;
    }
    type get() const {
        mbed::CriticalSectionLock lck;
        return value;
    }
    void set(type other) {
        mbed::CriticalSectionLock lck;
        value = other;
    }
    type xchg(type other) {
        mbed::CriticalSectionLock lck;
        type copy = value;
        value = other;
        return copy;
    }
    void add_isr(type other) { value += other; }
};
#else
template <class T>
struct AtomicPosition {
    using type = T;
    volatile type value;

    AtomicPosition(T t) : value {t} {}
    AtomicPosition(const AtomicPosition &o) : AtomicPosition {o.get()} {}
    AtomicPosition(AtomicPosition &&o) : AtomicPosition {o.get()} {}
    AtomicPosition &operator=(const AtomicPosition &o) {
        this->set(o.get());
        return *this;
    }
    AtomicPosition &operator=(AtomicPosition &&o) {
        this->set(o.get());
        return *this;
    }

    void add(type other) {
        noInterrupts();
        value += other;
        interrupts();
    }
    type get() const {
        type copy;
        noInterrupts();
        copy = value;
        interrupts();
        return copy;
    }
    void set(type other) {
        noInterrupts();
        value = other;
        interrupts();
    }
    type xchg(type other) {
        type copy;
        noInterrupts();
        copy = value;
        value = other;
        interrupts();
        return copy;
    }
    void add_isr(type other) { value += other; }
};
#endif

END_CS_NAMESPACE