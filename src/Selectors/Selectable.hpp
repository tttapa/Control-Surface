#pragma once

#include <stdint.h>

using setting_t = uint8_t;

template <setting_t N> // TODO: check bounds here?
class Selectable {
    public:
        virtual void select(setting_t setting) = 0;
};