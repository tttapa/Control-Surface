#pragma once

#include <Arduino.h>
#include <AH/Settings/NamespaceSettings.hpp>

BEGIN_AH_NAMESPACE

#ifdef min
#undef min
#endif
/// Return the smaller of two numbers/objects.
/// @ingroup    AH_Math
template <class T, class U>
constexpr auto min(const T &a, const U &b) -> decltype(b < a ? b : a) {
    return b < a ? b : a;
}

#ifdef max
#undef max
#endif
/// Return the larger of two numbers/objects.
/// @ingroup    AH_Math
template <class T, class U>
constexpr auto max(const T &a, const U &b) -> decltype(a < b ? b : a) {
    return a < b ? b : a;
}

END_AH_NAMESPACE