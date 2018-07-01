#ifndef DEBUG

#ifdef ARDUINO

// #define DEBUG_OUT Serial // Uncomment this line to override Arduino IDE debug level

#ifndef PRINTSTREAM_FALLBACK
#include "PrintStream.h"
#else
#include <Arduino.h>
typedef Print &manipulator(Print &);
inline Print &endl(Print &printer) {
  printer.println();
  printer.flush();
  return printer;
}
template<class T> inline Print &operator<<(Print &printer, const T printable) {
  printer.print(printable);
  return printer;
}
template<> inline Print &operator<<(Print &printer, manipulator pf) {
  return pf(printer);
}
#endif

#else // No Arduino

#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;
using std::setbase;
using std::setprecision;
using std::dec;
using std::hex;
using std::flush;
using std::boolalpha;
using std::noboolalpha;
using std::uppercase;
using std::nouppercase;
using std::showbase;
using std::noshowbase;
#define F(x) x

#endif

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define FUNC_LOCATION '[' << __PRETTY_FUNCTION__ << F(" @ line " STR(__LINE__) "]:\t")
#define LOCATION "[" __FILE__ ":" STR(__LINE__) "]:\t"

#ifdef DEBUG_OUT

#pragma message("Debugging enabled on output " STR(DEBUG_OUT))

#define DEBUG(x) do { \
  DEBUG_OUT << x << endl; \
} while (0)
#define DEBUGREF(x) do { \
  DEBUG_OUT << F(LOCATION) << x << endl; \
} while (0)
#define DEBUGFN(x) do { \
  DEBUG_OUT << FUNC_LOCATION << x << endl; \
} while (0)

#else

#define DEBUG(x)
#define DEBUGREF(x)
#define DEBUGFN(x)

#endif

#endif