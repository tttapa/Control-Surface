#pragma once

/**
 * @file
 * @brief   AVR Arduinos don't have access to the STL, so this is a simple
 *          implementation of some of the SFINAE stuff used by other parts of 
 *          the library.
 * 
 * <https://en.cppreference.com/w/cpp/types/enable_if>
 */

template <bool B, class T = void>
struct enable_if {};

template <class T>
struct enable_if<true, T> {
    typedef T type;
};

template <bool B, class T = void>
using enable_if_t = typename enable_if<B, T>::type;