#pragma once

#include <Settings/NamespaceSettings.hpp>

#include <AH/STL/type_traits>
#include <AH/STL/utility>

BEGIN_CS_NAMESPACE

template <class...>
using void_t = void;

template <class, class = void>
struct has_method_begin : std::false_type {};

template <class T>
struct has_method_begin<T, void_t<decltype(std::declval<T>().begin())>>
    : std::true_type {};

/// Calls the `begin()` method of `t` if that method exists.
template <class T>
typename std::enable_if<has_method_begin<T>::value>::type
begin_if_possible(T &t) {
    t.begin();
}

template <class T>
typename std::enable_if<!has_method_begin<T>::value>::type
begin_if_possible(T &) {}

END_CS_NAMESPACE
