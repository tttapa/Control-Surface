#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

AH_DIAGNOSTIC_EXTERNAL_HEADER()
#include <Arduino.h> // min max
AH_DIAGNOSTIC_POP()

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#ifdef abs
#undef abs
#endif

#ifdef round
#undef round
#endif

AH_DIAGNOSTIC_POP()
