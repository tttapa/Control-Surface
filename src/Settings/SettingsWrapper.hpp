#ifndef SETTINGSWRAPPER_HPP
#define SETTINGSWRAPPER_HPP

// ---- User Settings ---- //
// ======================= //
#include "Settings.hpp"

#ifndef ARDUINO
#undef IGNORE_SYSEX
#ifdef DEBUG_OUT
#undef DEBUG_OUT
#endif
#ifndef NO_DEBUG_PRINTS
#define DEBUG_OUT std::cout
#endif
#endif

#ifdef TEENSYDUINO
#include <Helpers/TeensyUSBTypes.hpp>
#if defined(DEBUG_OUT) && (DEBUG_OUT == Serial) &&     \
    !defined(TEENSY_SERIALUSB_ENABLED)
#error                                                                         \
    "Debugging is enabled on the CDC Serial port, but the USB type doesn't support Serial"
#endif
#endif

// ------- Debug ------- //
// ===================== //
#include <Helpers/Debug.hpp>

#endif // SETTINGSWRAPPER_HPP
