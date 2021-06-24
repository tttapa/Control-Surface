/**
 * @file
 * @brief	Conversions between radians and degrees.
 */
#pragma once

#include <AH/STL/type_traits>
#include <AH/Settings/NamespaceSettings.hpp>
#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

BEGIN_AH_NAMESPACE

namespace detail {
constexpr long double pi_inv_l = 0.318309886183790671537767526745028724L;
constexpr long double pi_l = 3.141592653589793238462643383279502884L;
} // namespace detail

/// @addtogroup AH_Math
/// @{

/// Convert radians to degrees.
template <class T>
constexpr inline
    typename std::enable_if<std::is_floating_point<T>::value, T>::type
    rad2deg(T r) {
    return r * static_cast<T>(detail::pi_inv_l) * 180;
}
/// Convert degrees to radians.
template <class T>
constexpr inline
    typename std::enable_if<std::is_floating_point<T>::value, T>::type
    deg2rad(T d) {
    return d * static_cast<T>(detail::pi_l) / 180;
}

/// Convert degrees to radians, e.g. 10_deg.
constexpr long double operator"" _deg(long double deg) { return deg2rad(deg); }
/// Convert degrees to radians, e.g. 10_deg.
constexpr long double operator"" _deg(unsigned long long deg) {
    return deg2rad(static_cast<long double>(deg));
}

/// @}

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
