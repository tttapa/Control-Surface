#pragma once

#include <stdint.h>

using setting_t = uint8_t;

class Selectable {
    public:
        virtual void select(setting_t setting) = 0;
};