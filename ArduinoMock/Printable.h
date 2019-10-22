#ifndef Printable_h
#define Printable_h

#include <stddef.h>

class Print;

class Printable {
  public:
    virtual size_t printTo(Print &p) const = 0;
};

#include <Print.h>

inline std::ostream &operator<<(std::ostream &os, const Printable &p) {
    OstreamPrint op = os;
    p.printTo(op);
    return os;
}

#endif