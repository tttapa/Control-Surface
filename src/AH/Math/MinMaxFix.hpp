#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/Math/FixArduinoMacros.hpp>

#include <AH/Settings/NamespaceSettings.hpp>

BEGIN_AH_NAMESPACE

/// Return the smaller of two numbers/objects.
/// @ingroup    AH_Math
template <class T, class U>
constexpr auto min(const T &a, const U &b) -> decltype(b < a ? b : a) {
    return b < a ? b : a;
}

/// Return the larger of two numbers/objects.
/// @ingroup    AH_Math
template <class T, class U>
constexpr auto max(const T &a, const U &b) -> decltype(a < b ? b : a) {
    return a < b ? b : a;
}

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
