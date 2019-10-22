#ifndef SETTINGSWRAPPER_HPP
#error "Do not include this file directly, use the wrapper!"
#endif

/**
 * @file Settings.hpp
 *
 * @brief All user settings and debugging options can be changed here.
 */

#include <Def/Frequency.hpp>
#include <Helpers/ADCConfig.hpp>
#include <limits.h> // CHAR_BIT
#include <stddef.h>
#include <stdint.h>

BEGIN_CS_NAMESPACE

// ----------------------------- Debug Settings ----------------------------- //
// ========================================================================== //

/// The debug output.
// #define DEBUG_OUT Serial

// #define DEBUG_MIDI_PACKETS

/// Exit when encountering an error, instead of trying to recover (recommended).
#define FATAL_ERRORS

// ----------------------------- User Settings ------------------------------ //
// ========================================================================== //

/// The default baud rate for debug MIDI interfaces.
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
 * resolution to @ref Helpers/ADCConfig.hpp.
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

/// The time in milliseconds it takes for the VU meter display peak bar to drop
///  one unit (i.e. one twelfth of the complete scale).
constexpr unsigned long VU_PEAK_DECAY_TIME = 300; // milliseconds

/// The time in milliseconds that a VU meter display peak bar stays at the peak
/// before decaying.
constexpr unsigned long VU_PEAK_HOLD_TIME = 400; // milliseconds

/// If set to true, the VU meter display peak bars will decay smoothly (i.e. one
/// pixel at a time), if set to false, they will decay one unit at a time. */
constexpr bool VU_PEAK_SMOOTH_DECAY = true;

/// Determines when a note input should be interpreted as 'on'.
constexpr uint8_t NOTE_VELOCITY_THRESHOLD = 1;

/// Don't parse incoming System Exclusive messages.
#define IGNORE_SYSEX 0

/** The length of the maximum System Exclusive message
 *  that can be received. The maximum length sent by
 *  the MCU protocol is 120 bytes.
 */
constexpr size_t SYSEX_BUFFER_SIZE = 128;

/// The baud rate to use for Hairless MIDI.
constexpr unsigned long HAIRLESS_BAUD = 115200;

/// The maximum frame rate of the displays.
constexpr uint8_t MAX_FPS = 60;

constexpr static Frequency SPI_MAX_SPEED = 8_MHz;

/// Make it possible to invert individual push buttons.
/// Enabling this will increase memory usage.
#define INDIVIDUAL_BUTTON_INVERT

// ========================================================================== //

END_CS_NAMESPACE