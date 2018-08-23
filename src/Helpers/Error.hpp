#pragma once

#include "Debug.hpp"

#ifdef ARDUINO // ------------------------------------------------------ ARDUINO

extern void fatalErrorExit() __attribute__((noreturn));

#ifdef FATAL_ERRORS

#define ERROR(x)                                                               \
    do {                                                                       \
        DEBUGFN(x);                                                            \
        fatalErrorExit();                                                      \
    } while (0)

#else

#define ERROR(x)                                                               \
    do {                                                                       \
        DEBUGFN(x);                                                            \
    } while (0)

#endif

#define FATAL_ERROR(x)                                                         \
    do {                                                                       \
        DEBUGFN(x);                                                            \
        fatalErrorExit();                                                      \
    } while (0)

#else // ----------------------------------------------------------------- TESTS

#include <exception>
#include <sstream>

class ErrorException : public std::exception {
  public:
    ErrorException(const std::string message) : message(std::move(message)) {}
    const char *what() const throw() { return message.c_str(); }

  private:
    const std::string message;
};

#define ERROR(x)                                                               \
    do {                                                                       \
        std::ostringstream s;                                                  \
        s << FUNC_LOCATION << x;                                               \
        throw ErrorException(s.str());                                         \
    } while (0)

#define FATAL_ERROR(x)                                                         \
    do {                                                                       \
        std::ostringstream s;                                                  \
        s << FUNC_LOCATION << x;                                               \
        throw ErrorException(s.str());                                         \
    } while (0)

#endif