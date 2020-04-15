#include "Vector.hpp"

AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/PrintStream/PrintStream.hpp>
#ifndef ARDUINO
#include <ostream> // std::ostream, <<
#endif

BEGIN_AH_NAMESPACE

// LCOV_EXCL_START

#ifndef ARDUINO

/// Printing.
/// @related  Vec2f
std::ostream &operator<<(std::ostream &os, Vec2f v) {
    return os << "(" << v.x << ", " << v.y << ")";
}

/// Printing.
/// @related  Vec3f
std::ostream &operator<<(std::ostream &os, Vec3f v) {
    return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

#endif

/// Printing.
/// @related  Vec2f
Print &operator<<(Print &os, Vec2f v) {
    return os << "(" << v.x << ", " << v.y << ")";
}

/// Printing.
/// @related  Vec3f
Print &operator<<(Print &os, Vec3f v) {
    return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

// LCOV_EXCL_STOP

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
