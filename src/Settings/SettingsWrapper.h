#ifndef SETTINGSWRAPPER_H
#define SETTINGSWRAPPER_H

// ---- User Settings ---- //
// ======================= //
#include "Settings.h"

#ifndef ARDUINO
#undef IGNORE_SYSEX
#ifdef DEBUG_OUT
#undef DEBUG_OUT
#endif
#ifndef NO_DEBUG_PRINTS
#define DEBUG_OUT std::cout
#endif
#endif

// ------- Debug ------- //
// ===================== //
#include <Helpers/Debug.hpp>

#endif // SETTINGSWRAPPER_H
