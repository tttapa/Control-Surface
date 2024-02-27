#include "Quaternion.hpp"

#include <AH/PrintStream/PrintStream.hpp>
#ifndef ARDUINO
#include <ostream> // std::ostream, <<
#endif

BEGIN_AH_NAMESPACE

// LCOV_EXCL_START

#ifndef ARDUINO

std::ostream &operator<<(std::ostream &os, Quaternion q) {
    return os << "(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
}

std::ostream &operator<<(std::ostream &os, EulerAngles e) {
    os << "(" << rad2deg(e.yaw) << "°, " << rad2deg(e.pitch) << "°, "
       << rad2deg(e.roll) << "°)";
    return os;
}

#endif // ARDUINO

Print &operator<<(Print &os, Quaternion q) {
    return os << "(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
}

Print &operator<<(Print &os, EulerAngles e) {
    os << "(" << rad2deg(e.yaw) << "°, " << rad2deg(e.pitch) << "°, "
       << rad2deg(e.roll) << "°)";
    return os;
}

// LCOV_EXCL_STOP

END_AH_NAMESPACE
