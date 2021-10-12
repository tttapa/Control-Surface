#ifndef AH_SETTINGSWRAPPER_HPP
#error "Do not include this file directly, use the wrapper!"
#endif

/**
 * @file
 * @brief   All user settings and debugging options can be changed here.
 * @ingroup AH_Settings
 */

#include <AH/Types/Frequency.hpp>
#include <AH/Hardware/ADCConfig.hpp>
#include <limits.h> // CHAR_BIT
#include <stddef.h>
#include <stdint.h>

BEGIN_AH_NAMESPACE

// ----------------------------- Debug Settings ----------------------------- //
// ========================================================================== //

#ifndef DEBUG_OUT
/// The debug output.  
/// Use `#define DEBUG_OUT Serial` to print debugging information to the serial
/// monitor.  
/// @note   Printing debug information can slow down the program dramatically.
#define DEBUG_OUT
// #define DEBUG_OUT Serial
#endif

/// Exit when encountering an error, instead of trying to recover (recommended).
#define FATAL_ERRORS

// ----------------------------- User Settings ------------------------------ //
// ========================================================================== //

/// The default baud rate for debug output.
constexpr unsigned long defaultBaudRate = 115200;

/**
 * The bit depth to use for the ADC (Analog to Digital Converter).
 * 
 * By default, the maximum supported resolution is used, but if you need it for
 * compatibility with other code that expects the default 10-bit resolution, you
 * can use 
 * 
 * ```cpp
 * constexpr uint8_t ADC_BITS = 10;
 * ```
 * 
 * If the library doesn't know your specific hardware, it defaults to 10 bits.
 * This might not be the optimal resolution, so it's best to add the actual 
 * resolution to @ref Hardware/ADCConfig.hpp.
 */
constexpr uint8_t ADC_BITS = ADC_RESOLUTION;

/** 
 * The factor for the analog filter:  
 * Difference equation:
 * @f$ y[n] = \alpha\cdot x[n] + (1-\alpha)\cdot y[n-1] @f$
 * where
 * @f$ \alpha = \left(\frac{1}{2}\right)^{ANALOG\_FILTER\_SHIFT\_FACTOR} @f$
 * 
 * @see FilteredAnalog
 */
constexpr uint8_t ANALOG_FILTER_SHIFT_FACTOR = 2;

/** 
 * The unsigned integer type to use for analog inputs during filtering.
 * 
 * @see FilteredAnalog
 */
using ANALOG_FILTER_TYPE = uint16_t;

/// The debounce time for momentary push buttons in milliseconds.
constexpr unsigned long BUTTON_DEBOUNCE_TIME = 25; // milliseconds

/// The time in milliseconds before a press is registered as a long press.
constexpr unsigned long LONG_PRESS_DELAY = 450; // milliseconds

/// The time between increments/decremnets during a long press.
constexpr unsigned long LONG_PRESS_REPEAT_DELAY = 200; // milliseconds

/// The interval between updating filtered analog inputs, in microseconds.
constexpr unsigned long FILTERED_INPUT_UPDATE_INTERVAL = 1000; // microseconds

constexpr static Frequency SPI_MAX_SPEED = 8_MHz;

// ========================================================================== //

END_AH_NAMESPACE