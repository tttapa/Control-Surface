#pragma once

#include <Settings/SettingsWrapper.hpp>

#if !(defined(ESP32) || defined(ESP8266))
// Uncomment this line to flush the output after each debug statement
#define FLUSH_ON_EVERY_DEBUG_STATEMENT
// TODO: I should probably use Streams instead of Prints, so Espressif boards
// can flush as well.
#endif

#ifdef FLUSH_ON_EVERY_DEBUG_STATEMENT
#define ENDL endl
#else
#define ENDL "\r\n"
#endif

#ifdef ARDUINO

// Uncomment this line to override Arduino IDE debug output
// #define DEBUG_OUT Serial

#if (defined(ESP32) || defined(ESP8266)) &&                                    \
    defined(FLUSH_ON_EVERY_DEBUG_STATEMENT)
#error "ESP32 and ESP8266 don't support flushing `Print` objects"
#endif

#ifndef PRINTSTREAM_FALLBACK
#include <PrintStream/PrintStream.hpp>
#else
#include <Arduino.h> // Print
typedef Print &manipulator(Print &);
inline Print &endl(Print &printer) {
    printer.println();
#if !(defined(ESP32) || defined(ESP8266))
    printer.flush();
#endif
    return printer;
}
template <class T>
inline Print &operator<<(Print &printer, const T printable) {
    printer.print(printable);
    return printer;
}
template <>
inline Print &operator<<(Print &printer, manipulator pf) {
    return pf(printer);
}
#endif

#else // No Arduino

#include <Arduino.h>
#include <PrintStream/PrintStream.hpp>

#endif

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define FUNC_LOCATION                                                          \
    '[' << __PRETTY_FUNCTION__ << F(" @ line " STR(__LINE__) "]:\t")
#define LOCATION "[" __FILE__ ":" STR(__LINE__) "]:\t"

#define NAMEDVALUE(x) F(STR(x) " = ") << x

#ifdef DEBUG_OUT

#pragma message("Debugging enabled on output " STR(DEBUG_OUT))

#define DEBUG(x)                                                               \
    do {                                                                       \
        DEBUG_OUT << x << ENDL;                                                \
    } while (0)

#define DEBUGREF(x)                                                            \
    do {                                                                       \
        DEBUG_OUT << F(LOCATION) << x << ENDL;                                 \
    } while (0)

#define DEBUGFN(x)                                                             \
    do {                                                                       \
        DEBUG_OUT << FUNC_LOCATION << x << ENDL;                               \
    } while (0)

#ifdef ARDUINO
#define DEBUGTIME(x)                                                           \
    do {                                                                       \
        unsigned long t = millis();                                            \
        unsigned long h = t / (60UL * 60 * 1000);                              \
        unsigned long m = (t / (60UL * 1000)) % 60;                            \
        unsigned long s = (t / (1000UL)) % 60;                                 \
        unsigned long ms = t % 1000;                                           \
        const char *ms_zeros = ms > 99 ? "" : (ms > 9 ? "0" : "00");           \
        DEBUG_OUT << '[' << h << ':' << m << ':' << s << '.' << ms_zeros << ms \
                  << "]:\t" << x << ENDL;                                      \
    } while (0)
#endif

#include "DebugVal.hpp"

#define DEBUGVAL(...) DEBUGVALN(COUNT(__VA_ARGS__))(__VA_ARGS__)

#else // Debugging disabled

#define DEBUG(x)                                                               \
    do {                                                                       \
    } while (0)
#define DEBUGREF(x)                                                            \
    do {                                                                       \
    } while (0)
#define DEBUGFN(x)                                                             \
    do {                                                                       \
    } while (0)
#ifdef ARDUINO
#define DEBUGTIME(x)                                                           \
    do {                                                                       \
    } while (0)
#endif
#define DEBUGVAL(...)                                                          \
    do {                                                                       \
    } while (0)

#endif