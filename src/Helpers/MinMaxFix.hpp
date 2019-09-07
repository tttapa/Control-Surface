#pragma once

#include <Arduino.h>

#ifdef min
#undef min
template <class T, class U>
auto min(const T &a, const U &b) -> decltype(b < a ? b : a) {
    return b < a ? b : a;
}
#endif

#ifdef max
#undef max
template <class T, class U>
auto max(const T &a, const U &b) -> decltype(b < a ? b : a) {
    return a < b ? b : a;
}
#endif