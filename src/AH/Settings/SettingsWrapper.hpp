#ifndef AH_SETTINGSWRAPPER_HPP
#define AH_SETTINGSWRAPPER_HPP

#include "NamespaceSettings.hpp"

// ---- User Settings ---- //
// ======================= //
#include "Settings.hpp"

AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#define AH_IS_EMPTY_HELPER(x) x##1
#define AH_IS_EMPTY(x) AH_IS_EMPTY_HELPER(x) == 1

#if AH_IS_EMPTY(DEBUG_OUT)
#undef DEBUG_OUT
#endif

#ifndef ARDUINO
#ifdef DEBUG_OUT
#undef DEBUG_OUT
#ifndef NO_DEBUG_PRINTS
#define DEBUG_OUT std::cout
#endif
#endif
#endif

#ifdef TEENSYDUINO
#include <AH/Teensy/TeensyUSBTypes.hpp>
#if defined(DEBUG_OUT) && (DEBUG_OUT == Serial) &&                             \
    !defined(TEENSY_SERIALUSB_ENABLED)
#error                                                                         \
    "Debugging is enabled on the CDC Serial port, but the USB type doesn't support Serial"
#endif
#endif

BEGIN_AH_NAMESPACE
static_assert(
    sizeof(ANALOG_FILTER_TYPE) * CHAR_BIT >=
        ADC_BITS + ANALOG_FILTER_SHIFT_FACTOR,
    "ANALOG_FILTER_TYPE isn't wide enough to satisfy filter requirements. \n"
    "Either decrease the ADC resolution, decrease the filter shift factor, or"
    "use a wider type (e.g. uint32_t)");
END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()

// ------- Debug ------- //
// ===================== //
#include <AH/Debug/Debug.hpp>

#endif // AH_SETTINGSWRAPPER_HPP
