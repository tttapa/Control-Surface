#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/STL/cmath>  // M_PI

BEGIN_AH_NAMESPACE

/// @addtogroup AH_Math
/// @{

/**
 * @file
 * @brief	Conversions between radians and degrees.
 */

/// Convert radians to degrees.
template <class T>
constexpr inline double rad2deg(T r) {
    return r * M_1_PI * 180;
}
/// Convert degrees to radians.
template <class T>
constexpr inline double deg2rad(T d) {
    return d * M_PI / 180;
}

/// Convert degrees to radians, e.g. 10_deg.
constexpr long double operator"" _deg(long double deg) { return deg2rad(deg); }
/// Convert degrees to radians, e.g. 10_deg.
constexpr long double operator"" _deg(unsigned long long deg) {
    return deg2rad<long double>(deg);
}

/// @}

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
