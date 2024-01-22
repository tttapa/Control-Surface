#pragma once

#include <Settings/NamespaceSettings.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

BEGIN_CS_NAMESPACE

#if __cplusplus >= 201703L

namespace compat {
using std::byte;
using std::in_place;
using std::in_place_t;
using std::in_place_type;
using std::in_place_type_t;
} // namespace compat

#else

namespace compat {
struct in_place_t {
    explicit in_place_t() = default;
};
static in_place_t in_place {};
template <class T>
struct in_place_type_t {
    explicit in_place_type_t() = default;
};
template <class T>
static in_place_type_t<T> in_place_type {};
enum class byte : unsigned char {};
} // namespace compat

#endif

#if __cplusplus >= 202002L

namespace compat {
using std::remove_cvref;
} // namespace compat

#else

namespace compat {
template <class T>
struct remove_cvref {
    using type =
        typename std::remove_cv<typename std::remove_reference<T>::type>::type;
};
} // namespace compat

#endif

END_CS_NAMESPACE
