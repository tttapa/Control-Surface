#ifndef SETTINGSWRAPPER_H
#define SETTINGSWRAPPER_H

// ---- User Settings ---- //
// ======================= //

#include "./Settings.h"

// ---- Tests on PC ---- //
// ===================== //

#ifndef ARDUINO
#ifdef DEBUG
#undef DEBUG
#define DEBUG cout
#endif
#endif

#endif // SETTINGSWRAPPER_H
