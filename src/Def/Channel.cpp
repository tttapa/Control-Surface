#include "Channel.hpp"
#include <AH/PrintStream/PrintStream.hpp>

BEGIN_CS_NAMESPACE

Print &operator<<(Print &os, Channel c) {
    return os << F("Channel ") << c.getOneBased();
}

END_CS_NAMESPACE