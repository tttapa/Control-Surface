#pragma once

/// Mutex that doesn't lock anything, used on single-core systems.
struct EmptyMutex {
    void lock() { /* nothing */ }
    void unlock() { /* nothing */ }
};

/// Lock guard that doesn't lock it's mutex, used on single-core systems.
template <class Mutex>
struct EmptyLockGuard {
    EmptyLockGuard(Mutex &) { /* nothing */ }
    ~EmptyLockGuard() { /* nothing */ }
};

#if defined(ESP32) || !defined(ARDUINO)

#include <mutex>

using DefaultMutEx = std::mutex;
template <class Mutex>
using DefaultLockGuard = std::lock_guard<Mutex>;

#else

using DefaultMutEx = EmptyMutex;
template <class Mutex>
using DefaultLockGuard = EmptyLockGuard<Mutex>;

#endif