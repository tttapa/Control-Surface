#pragma once

/// @file

#include <AH/Debug/Debug.hpp>

#ifdef ARDUINO // ------------------------------------------------------ ARDUINO

BEGIN_AH_NAMESPACE

/// Function that executes and loops forever, blinking the built-in LED when a
/// fatal error is encountered.
extern void fatalErrorExit() __attribute__((noreturn));

END_AH_NAMESPACE

#ifdef FATAL_ERRORS

#define ERROR(msg, errc)                                                       \
    do {                                                                       \
        USING_AH_NAMESPACE;                                                    \
        DEBUGFN(msg << " (0x" << hex << uppercase << errc << dec               \
                    << nouppercase << ')');                                    \
        fatalErrorExit();                                                      \
    } while (0)

#else

/// Print the error message and error code, and stop the execution if
/// `FATAL_ERRORS` are enabled. Otherwise just prints the error.
///
/// @param  msg
///         The information to print, can contain streaming operators (`<<`) to
///         print multiple things.
/// @param  errc
///         A unique error code.
///
/// @ingroup    AH_Error
#define ERROR(msg, errc)                                                       \
    do {                                                                       \
        DEBUGFN(msg << " (0x" << hex << uppercase << errc << dec               \
                    << nouppercase << ')');                                    \
    } while (0)

#endif

/// Print the error message and error code, and stop the execution.
/// Doesn't depend on `FATAL_ERRORS`, it always stops the execution.
///
/// @param  msg
///         The information to print, can contain streaming operators (`<<`) to
///         print multiple things.
/// @param  errc
///         A unique error code.
///
/// @ingroup    AH_Error
#define FATAL_ERROR(msg, errc)                                                 \
    do {                                                                       \
        USING_AH_NAMESPACE;                                                    \
        DEBUGFN(F("Fatal Error: ") << msg << " (0x" << hex << uppercase        \
                                   << errc << dec << nouppercase << ')');      \
        fatalErrorExit();                                                      \
    } while (0)

#else // ----------------------------------------------------------------- TESTS

#include <exception>
#include <sstream>

BEGIN_AH_NAMESPACE

class ErrorException : public std::exception {
  public:
    ErrorException(const std::string message, int errorCode)
        : message(std::move(message)), errorCode(errorCode) {}
    const char *what() const throw() override { return message.c_str(); }
    int getErrorCode() const { return errorCode; }

  private:
    const std::string message;
    const int errorCode;
};

END_AH_NAMESPACE

#define ERROR(msg, errc)                                                       \
    do {                                                                       \
        USING_AH_NAMESPACE;                                                    \
        std::ostringstream s;                                                  \
        s << DEBUG_FUNC_LOCATION << msg;                                       \
        throw ErrorException(s.str(), errc);                                   \
    } while (0)

#define FATAL_ERROR(msg, errc)                                                 \
    do {                                                                       \
        USING_AH_NAMESPACE;                                                    \
        std::ostringstream s;                                                  \
        s << DEBUG_FUNC_LOCATION << msg;                                       \
        throw ErrorException(s.str(), errc);                                   \
    } while (0)

#endif
