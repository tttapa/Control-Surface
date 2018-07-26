#pragma once

#include <Helpers/Array.hpp>
#include <stdint.h> // uint8_t
#include <stddef.h> // size_t

typedef uint16_t pin_t;
typedef int16_t analog_t;

template <size_t N>
using PinList = Array<pin_t, N>;

template <class T, size_t nb_rows, size_t nb_cols>
using Array2D = Array<Array<T, nb_cols>, nb_rows>;