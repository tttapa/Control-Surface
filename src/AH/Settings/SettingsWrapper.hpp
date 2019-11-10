#ifndef SETTINGSWRAPPER_HPP
#define SETTINGSWRAPPER_HPP

// ---- User Settings ---- //
// ======================= //
#include "NamespaceSettings.hpp"
#include "Settings.hpp"

#ifndef ARDUINO
#ifdef DEBUG_OUT
#undef DEBUG_OUT
#endif
#ifndef NO_DEBUG_PRINTS
#define DEBUG_OUT std::cout
#endif
#endif

#ifdef INDIVIDUAL_BUTTON_INVERT
#define INDIVIDUAL_BUTTON_INVERT_STATIC
#else
#define INDIVIDUAL_BUTTON_INVERT_STATIC static
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

// ------- Debug ------- //
// ===================== //
#include <AH/Debug/Debug.hpp>

#endif // SETTINGSWRAPPER_HPP
