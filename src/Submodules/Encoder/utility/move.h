#pragma once


#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

namespace enc_util {

template <class T> struct remove_reference       { typedef T type; };
template <class T> struct remove_reference<T &>  { typedef T type; };
template <class T> struct remove_reference<T &&> { typedef T type; };

template <class T>
typename remove_reference<T>::type &&
move(T &&t) {
	return static_cast<typename remove_reference<T>::type &&>(t);
}

template<typename T>
void swap(T &t1, T &t2) {
    T temp = move(t1);
    t1 = move(t2);
    t2 = move(temp);
}

} // namespace enc_util

END_CS_NAMESPACE
