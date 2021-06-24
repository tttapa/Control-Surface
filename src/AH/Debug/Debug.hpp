#pragma once

/// @file

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/PrintStream/PrintStream.hpp>
#include <AH/Settings/SettingsWrapper.hpp>

#ifndef FLUSH_ON_EVERY_DEBUG_STATEMENT
#if !(defined(ESP32) || defined(ESP8266))

/// Should the output stream be flushed after each debug statement?
/// Enabling this feature can slow things down significantly, and is not
/// supported on ESP32 / ESP8266.
///
/// @todo   I should probably use Streams instead of Prints, so Espressif boards
///         can flush as well.
#define FLUSH_ON_EVERY_DEBUG_STATEMENT 0

#else

#define FLUSH_ON_EVERY_DEBUG_STATEMENT 0

#endif
#endif

#if defined(ESP32)
#include <mutex>
#elif defined(ARDUINO_ARCH_MBED)
#include <mutex>
#include <rtos/Mutex.h>
#endif

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#ifdef ARDUINO

// Uncomment this line to override Arduino debug output
// #define DEBUG_OUT Serial

#else

// Uncomment this line to override PC tests debug output
// #define DEBUG_OUT std::cout

#endif

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#if FLUSH_ON_EVERY_DEBUG_STATEMENT
#define DEBUG_ENDL endl
#else
#define DEBUG_ENDL "\r\n"
#endif

#if (defined(ESP32) || defined(ESP8266)) && FLUSH_ON_EVERY_DEBUG_STATEMENT
#error "ESP32 and ESP8266 don't support flushing `Print` objects"
#endif

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#define DEBUG_STR_HELPER(x) #x
#define DEBUG_STR(x) DEBUG_STR_HELPER(x)

#define DEBUG_FUNC_LOCATION                                                    \
    '[' << __PRETTY_FUNCTION__ << F(" @ line " DEBUG_STR(__LINE__) "]:\t")
#define DEBUG_LOCATION "[" __FILE__ ":" DEBUG_STR(__LINE__) "]:\t"

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#if defined(ESP32)
#define DEBUG_LOCK_MUTEX std::lock_guard<std::mutex> lock(AH::debugmutex);
BEGIN_AH_NAMESPACE
extern std::mutex debugmutex;
END_AH_NAMESPACE
#elif defined(ARDUINO_ARCH_MBED)
#define DEBUG_LOCK_MUTEX std::lock_guard<rtos::Mutex> lock(AH::debugmutex);
BEGIN_AH_NAMESPACE
extern rtos::Mutex debugmutex;
END_AH_NAMESPACE
#else
#define DEBUG_LOCK_MUTEX
#endif

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/// Macro for printing an expression as a string, followed by its value.
/// The expression string is saved in PROGMEM using the `F(...)` macro.
/// @ingroup    AH_Debug
#define NAMEDVALUE(x) F(DEBUG_STR(x) " = ") << x

#ifdef DEBUG_OUT // Debugging enabled ==========================================

/// Print an expression to the debug output if debugging is enabled.
/// @ingroup    AH_Debug
#define DEBUG(x)                                                               \
    do {                                                                       \
        DEBUG_LOCK_MUTEX                                                       \
        DEBUG_OUT << x << DEBUG_ENDL;                                          \
    } while (0)

/// Print an expression and its location (file and line number) to the debug
/// output if debugging is enabled.
/// The location is saved in PROGMEM using the `F(...)` macro.
/// @ingroup    AH_Debug
#define DEBUGREF(x)                                                            \
    do {                                                                       \
        DEBUG_LOCK_MUTEX                                                       \
        DEBUG_OUT << F(DEBUG_LOCATION) << x << DEBUG_ENDL;                     \
    } while (0)

/// Print an expression and its function (function name and line number) to the
/// debug output if debugging is enabled.
/// The function name is saved in RAM.
/// @ingroup    AH_Debug
#define DEBUGFN(x)                                                             \
    do {                                                                       \
        DEBUG_LOCK_MUTEX                                                       \
        DEBUG_OUT << DEBUG_FUNC_LOCATION << x << DEBUG_ENDL;                   \
    } while (0)

#ifdef ARDUINO

/// Print an expression and the time since startup to the debug output if
/// debugging is enabled.
/// Format: `[hours:minutes:seconds.milliseconds]`
/// @ingroup    AH_Debug
#define DEBUGTIME(x)                                                           \
    do {                                                                       \
        DEBUG_LOCK_MUTEX                                                       \
        unsigned long t = millis();                                            \
        unsigned long h = t / (60UL * 60 * 1000);                              \
        unsigned long m = (t / (60UL * 1000)) % 60;                            \
        unsigned long s = (t / (1000UL)) % 60;                                 \
        unsigned long ms = t % 1000;                                           \
        const char *ms_zeros = ms > 99 ? "" : (ms > 9 ? "0" : "00");           \
        DEBUG_OUT << '[' << h << ':' << m << ':' << s << '.' << ms_zeros << ms \
                  << "]:\t" << x << DEBUG_ENDL;                                \
    } while (0)

#else // !ARDUINO

#include <chrono>

BEGIN_AH_NAMESPACE
extern const decltype(std::chrono::high_resolution_clock::now()) start_time;
END_AH_NAMESPACE

#define DEBUGTIME(x)                                                           \
    do {                                                                       \
        USING_AH_NAMESPACE;                                                    \
        using namespace std::chrono;                                           \
        auto now = high_resolution_clock::now();                               \
        unsigned long t =                                                      \
            duration_cast<milliseconds>(now - start_time).count();             \
        unsigned long h = t / (60UL * 60 * 1000);                              \
        unsigned long m = (t / (60UL * 1000)) % 60;                            \
        unsigned long s = (t / (1000UL)) % 60;                                 \
        unsigned long ms = t % 1000;                                           \
        const char *ms_zeros = ms > 99 ? "" : (ms > 9 ? "0" : "00");           \
        DEBUG_OUT << '[' << h << ':' << m << ':' << s << '.' << ms_zeros << ms \
                  << "]:\t" << x << DEBUG_ENDL;                                \
    } while (0)

#endif // ARDUINO

#include "DebugVal.hpp"

/// Print multiple expressions and their values to the debug output if debugging
/// is enabled.
/// For example, `DEBUGVAL(1 + 1, digitalRead(2))` could print `1 + 1 = 2,
/// digitalRead(2) = 0`.
/// A maximum of 10 expressions is supported.
/// The expression strings are saved in PROGMEM using the `F(...)` macro.
/// @ingroup    AH_Debug
#define DEBUGVAL(...) DEBUGVALN(COUNT(__VA_ARGS__))(__VA_ARGS__)

#else // Debugging disabled ====================================================

#define DEBUG(x)                                                               \
    do {                                                                       \
    } while (0)
#define DEBUGREF(x)                                                            \
    do {                                                                       \
    } while (0)
#define DEBUGFN(x)                                                             \
    do {                                                                       \
    } while (0)
#define DEBUGTIME(x)                                                           \
    do {                                                                       \
    } while (0)
#define DEBUGVAL(...)                                                          \
    do {                                                                       \
    } while (0)

#endif

AH_DIAGNOSTIC_POP()
