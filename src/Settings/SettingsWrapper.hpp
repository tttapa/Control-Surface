#ifndef CS_SETTINGSWRAPPER_HPP
#define CS_SETTINGSWRAPPER_HPP

// ---- User Settings ---- //
// ======================= //
#include "NamespaceSettings.hpp"
#include "Settings.hpp"

#ifndef ARDUINO
#undef IGNORE_SYSEX
#define IGNORE_SYSEX 0
#define MIDI_NUM_CABLES 16
#ifdef DEBUG_OUT
#undef DEBUG_OUT
#endif
#endif

#include <AH/Settings/SettingsWrapper.hpp>

#endif // CS_SETTINGSWRAPPER_HPP
