#include "Cable.hpp"
#include <AH/PrintStream/PrintStream.hpp>

BEGIN_CS_NAMESPACE

Print &operator<<(Print &os, Cable c) {
    return os << F("Cable ") << c.getOneBased();
}

END_CS_NAMESPACE