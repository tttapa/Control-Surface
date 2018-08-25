#pragma once

template <class T>
struct is_unsigned {
    constexpr static bool value = T(-1) > T(0);
};