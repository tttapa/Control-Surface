#ifndef SETTINGSWRAPPER_H
#error "Do not include this file directly, use the wrapper!"
#endif

/**
 * @file Settings.h
 *
 * @brief All user settings and debugging options can be changed here.
 */

#include <Def/Def.hpp>

// ----------------------------- Debug Settings ----------------------------- //
// ========================================================================== //

/** The debug output. */
// #define DEBUG_OUT Serial

// #define DEBUG_MIDI_PACKETS

/** Exit when encountering an error, instead of recovering. */
#define FATAL_ERRORS

// ----------------------------- User Settings ------------------------------ //
// ========================================================================== //

/** The factor for the analog filter: <br>
 *  Difference equation:
 *  @f$ y[n] = \alpha\cdot x[n] + (1-\alpha)\cdot y[n-1] @f$
 *  where
 *  @f$ \alpha = \left(\frac{1}{2}\right)^{ANALOG\_FILTER\_SHIFT\_FACTOR} @f$
 */
constexpr uint8_t ANALOG_FILTER_SHIFT_FACTOR = 2;

/** The signed integer type to use for analog inputs during filtering.
 * Should be at least @f$ 10+1+2·ANALOG\_FILTER\_SHIFT\_FACTOR @f$ bits wide.
 * (10 bits of ADC resolution, 1 sign bit, and 2·ANALOG_FILTER_SHIFT_FACTOR
 * bits of fixed point precision for intermediate filter calculations.
 */
typedef int16_t ANALOG_FILTER_TYPE;

/** The debounce time for momentary push buttons in milliseconds. */
constexpr unsigned long BUTTON_DEBOUNCE_TIME = 25; // milliseconds

/** The time in millisecoonds before a press is registered as a long press. */
constexpr unsigned long LONG_PRESS_DELAY = 450; // milliseconds

/** The time between increments/decremnets during a long press. */
constexpr unsigned long LONG_PRESS_REPEAT_DELAY = 200; // milliseconds

/** The interval between updating filtered analog inputs, in microseconds (µs).
 */
constexpr unsigned long FILTERED_INPUT_UPDATE_INTERVAL = 1000; // microseconds

/** The time in milliseconds it takes for the VU meter display peak bar to drop 
 *  one unit (i.e. one twelfth of the complete scale). */
constexpr unsigned long VU_PEAK_DECAY_TIME = 300; // milliseconds

/** The time in milliseconds that a VU meter display peak bar stays at the peak
 *  before decaying. */
constexpr unsigned long VU_PEAK_HOLD_TIME = 400; // milliseconds

/** If set to true, the VU meter display peak bars will decay smoothly (i.e. one
 *  pixel at a time), if set to false, they will decay one unit at a time. */
constexpr bool VU_PEAK_SMOOTH_DECAY = true;

/** Determines when a note input should be interpreted as 'on'. */
constexpr uint8_t NOTE_VELOCITY_THRESHOLD = 1;

/** Don't parse incomming System Exclusive messages. */
#define IGNORE_SYSEX

/** The length of the maximum System Exclusive message
 *  that can be received. The maximum length sent by
 *  the MCU protocol is 120 bytes.
 */
constexpr size_t SYSEX_BUFFER_SIZE = 128;

/** The baud rate to use for Hairless MIDI. */
constexpr unsigned long HAIRLESS_BAUD = 115200;

/** The maximum frame rate of the displays. */
constexpr uint8_t MAX_FPS = 30;

constexpr static Frequency SPI_MAX_SPEED = 8_MHz;

// ========================================================================== //