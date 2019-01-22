#ifndef Printable_h
#define Printable_h

#include <stddef.h>

class Print;

class Printable {
  public:
    virtual size_t printTo(Print& p) const = 0;
};

#endif
