#pragma once

#include <AH/Debug/Debug.hpp>

#ifdef ARDUINO // ------------------------------------------------------ ARDUINO

BEGIN_AH_NAMESPACE

/// Function that executes and loops forever, blinking the LED when a fatal
/// error is encountered.
extern void fatalErrorExit() __attribute__((noreturn));

END_AH_NAMESPACE

#ifdef FATAL_ERRORS

#define ERROR(x, e)                                                            \
    do {                                                                       \
        USING_AH_NAMESPACE;                                                    \
        DEBUGFN(x << " (0x" << hex << uppercase << e << dec << nouppercase     \
                  << ')');                                                     \
        fatalErrorExit();                                                      \
    } while (0)

#else

#define ERROR(x, e)                                                            \
    do {                                                                       \
        DEBUGFN(x << " (0x" << hex << uppercase << e << dec << nouppercase     \
                  << ')');                                                     \
    } while (0)

#endif

#define FATAL_ERROR(x, e)                                                      \
    do {                                                                       \
        DEBUGFN(F("Fatal Error: ") << x << " (0x" << hex << uppercase << e     \
                                   << dec << nouppercase << ')');              \
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

#define ERROR(x, e)                                                            \
    do {                                                                       \
        USING_AH_NAMESPACE;                                                    \
        std::ostringstream s;                                                  \
        s << DEBUG_FUNC_LOCATION << x;                                         \
        throw ErrorException(s.str(), e);                                      \
    } while (0)

#define FATAL_ERROR(x, e)                                                      \
    do {                                                                       \
        USING_AH_NAMESPACE;                                                    \
        std::ostringstream s;                                                  \
        s << DEBUG_FUNC_LOCATION << x;                                         \
        throw ErrorException(s.str(), e);                                      \
    } while (0)

#endif