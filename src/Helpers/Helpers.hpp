/* ✔ */

#pragma once

/** 
 * A utility class to check whether a given type is unsigned.
 * 
 * @tparam  T
 *          The type to check.
 */
template <class T>
struct is_unsigned {
    constexpr static bool value = T(-1) > T(0);
};