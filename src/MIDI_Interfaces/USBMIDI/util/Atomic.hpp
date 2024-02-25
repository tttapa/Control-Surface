#pragma once

#include <atomic>

#include <Settings/NamespaceSettings.hpp>

#if defined(ARDUINO_ARCH_RP2040)
#include <hardware/sync.h>
#elif defined(ARDUINO_ARCH_MBED)
#include <platform/mbed_critical.h>
#endif

BEGIN_CS_NAMESPACE

#ifndef ARDUINO

#define CS_USE_REAL_ATOMIC 1

#elif defined(ARDUINO_ARCH_RP2040)

class ScopedInterruptDisabler {
  public:
    ScopedInterruptDisabler() : state {save_and_disable_interrupts()} {}
    ~ScopedInterruptDisabler() { restore_interrupts(state); }
    ScopedInterruptDisabler(const ScopedInterruptDisabler &) = delete;
    ScopedInterruptDisabler &
    operator=(const ScopedInterruptDisabler &) = delete;
    ScopedInterruptDisabler(ScopedInterruptDisabler &&) = delete;
    ScopedInterruptDisabler &operator=(ScopedInterruptDisabler &&) = delete;

  private:
    uint32_t state;
};

#elif defined(ARDUINO_ARCH_MBED)

/// Temporarily disables interrupts and then restores them.
class ScopedInterruptDisabler {
  public:
    ScopedInterruptDisabler() { core_util_critical_section_enter(); }
    ~ScopedInterruptDisabler() { core_util_critical_section_exit(); }
    ScopedInterruptDisabler(const ScopedInterruptDisabler &) = delete;
    ScopedInterruptDisabler &
    operator=(const ScopedInterruptDisabler &) = delete;
    ScopedInterruptDisabler(ScopedInterruptDisabler &&) = delete;
    ScopedInterruptDisabler &operator=(ScopedInterruptDisabler &&) = delete;
};

#elif defined(TEENSYDUINO)

// Not necessary (assuming we only support the higher end boards)

#else
#error "Unknown platform, I don't know how to disable and restore interrupts"
#endif

// Some boards with a Cortex-M3, Cortex-M4 or Cortex-M7 have exclusive LD/STREX
// instructions, so they can implement read-modify-write operations without
// disabling interrupts, which turns out to be faster.

#if defined(DARDUINO_ARCH_NRF52840) || defined(DARDUINO_ARCH_MBED_GIGA) ||     \
    defined(TEENSYDUINO)
#define CS_USE_ATOMIC_RMW 1
#else
#define CS_USE_ATOMIC_RMW 0
#endif

// #define CS_USE_REAL_ATOMIC
#ifdef CS_USE_REAL_ATOMIC
template <class T>
using interrupt_atomic = std::atomic<T>;
#else

/// Wrapper that provides atomic access to variables shared between the main
/// program and interrupt handlers, by inserting the appropriate compile-time
/// fences. On chips that don't implement any atomic instructions in hardware,
/// like the Cortex-M0, interrupts may be disabled to ensure atomicity of read-
/// modify-write operations.
/// Interface derived from `std::atomic`.
template <class T>
class interrupt_atomic {
  public:
    interrupt_atomic() noexcept = default;
    explicit interrupt_atomic(T t) noexcept : value {t} {}

    [[gnu::always_inline]] static void
    after_load_fence(std::memory_order o) noexcept {
        switch (o) {
            case std::memory_order_consume: // fallthrough
            case std::memory_order_acq_rel: // fallthrough
            case std::memory_order_acquire:
                std::atomic_signal_fence(std::memory_order_acquire);
                break;
            case std::memory_order_seq_cst:
                std::atomic_signal_fence(std::memory_order_seq_cst);
                break;
            case std::memory_order_relaxed: // fallthrough
            case std::memory_order_release:
                // no fence needed
                break;
            default:;
        }
    }

    [[gnu::always_inline]] static void
    before_store_fence(std::memory_order o) noexcept {
        switch (o) {
            case std::memory_order_consume: // fallthrough
            case std::memory_order_acq_rel: // fallthrough
            case std::memory_order_acquire:
                std::atomic_signal_fence(std::memory_order_acquire);
                break;
            case std::memory_order_seq_cst:
                std::atomic_signal_fence(std::memory_order_seq_cst);
                break;
            case std::memory_order_relaxed: // fallthrough
            case std::memory_order_release:
                // no fence needed
                break;
            default:;
        }
    }

    [[gnu::always_inline]] T load(std::memory_order o) const {
        if (o == std::memory_order_seq_cst)
            std::atomic_signal_fence(std::memory_order_seq_cst);
        auto t = value.load(std::memory_order_relaxed);
        after_load_fence(o);
        return t;
    }

    [[gnu::always_inline]] void store(T t, std::memory_order o) noexcept {
        before_store_fence(o);
        value.store(t, std::memory_order_relaxed);
        if (o == std::memory_order_seq_cst)
            std::atomic_signal_fence(std::memory_order_seq_cst);
    }

#if CS_USE_ATOMIC_RMW
    [[gnu::always_inline]] T exchange(T arg, std::memory_order o) {
        before_store_fence(o);
        auto t = value.exchange(arg, std::memory_order_relaxed);
        after_load_fence(o);
        return t;
    }

    [[gnu::always_inline]] bool
    compare_exchange_strong(T &expected, T desired,
                            std::memory_order o) noexcept {
        before_store_fence(o);
        bool success = value.compare_exchange_strong(expected, desired,
                                                     std::memory_order_relaxed);
        after_load_fence(o);
        return success;
    }

    [[gnu::always_inline]] bool
    compare_exchange_weak(T &expected, T desired,
                          std::memory_order o) noexcept {
        before_store_fence(o);
        bool success = value.compare_exchange_weak(expected, desired,
                                                   std::memory_order_relaxed);
        after_load_fence(o);
        return success;
    }

    [[gnu::always_inline]] T fetch_add(T arg, std::memory_order o) {
        before_store_fence(o);
        auto t = value.fetch_add(arg, std::memory_order_relaxed);
        after_load_fence(o);
        return t;
    }

    [[gnu::always_inline]] T fetch_sub(T arg, std::memory_order o) {
        before_store_fence(o);
        auto t = value.fetch_sub(arg, std::memory_order_relaxed);
        after_load_fence(o);
        return t;
    }
#else
    [[gnu::always_inline]] T exchange(T arg, std::memory_order o) {
        ScopedInterruptDisabler disable_interrupts;
        before_store_fence(o);
        auto t = value.load(std::memory_order_relaxed);
        value.store(arg, std::memory_order_relaxed);
        after_load_fence(o);
        return t;
    }

    [[gnu::always_inline]] bool
    compare_exchange_strong(T &expected, T desired,
                            std::memory_order o) noexcept {
        ScopedInterruptDisabler disable_interrupts;
        if (o == std::memory_order_seq_cst)
            std::atomic_signal_fence(std::memory_order_seq_cst);
        auto t = value.load(std::memory_order_relaxed);
        bool success = t == expected;
        if (success) {
            before_store_fence(o);
            value.store(desired, std::memory_order_relaxed);
        } else {
            expected = t;
        }
        after_load_fence(o);
        return success;
    }

    [[gnu::always_inline]] bool
    compare_exchange_weak(T &expected, T desired,
                          std::memory_order o) noexcept {
        return compare_exchange_strong(expected, desired, o);
    }

    [[gnu::always_inline]] T fetch_add(T arg, std::memory_order o) {
        ScopedInterruptDisabler disable_interrupts;
        auto t = load(o);
        store(t + arg, o);
        return t;
    }

    [[gnu::always_inline]] T fetch_sub(T arg, std::memory_order o) {
        ScopedInterruptDisabler disable_interrupts;
        auto t = load(o);
        store(t - arg, o);
        return t;
    }
#endif

  private:
    std::atomic<T> value;
};

#endif

END_CS_NAMESPACE
