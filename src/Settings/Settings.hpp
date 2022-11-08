#ifndef CS_SETTINGSWRAPPER_HPP
#error "Do not include this file directly, use the wrapper!"
#endif

/**
 * @file
 * 
 * @brief   All user settings and debugging options for the Control Change 
 *          library can be changed here.
 */

#include "NamespaceSettings.hpp"
#include <stddef.h>
#include <stdint.h>

BEGIN_CS_NAMESPACE

// ----------------------------- Debug Settings ----------------------------- //
// ========================================================================== //

// #define DEBUG_MIDI_PACKETS

// ----------------------------- User Settings ------------------------------ //
// ========================================================================== //

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
/// Don't include code for sending System Exclusive messages.
#define NO_SYSEX_OUTPUT 0

/// The length of the maximum System Exclusive message that can be received.
/// The maximum length sent by the MCU protocol is 120 bytes.
constexpr uint16_t SYSEX_BUFFER_SIZE = 128;

/// Timeout in milliseconds to wait for a SysEx chunk to complete.
constexpr unsigned long SYSEX_CHUNK_TIMEOUT = 500;

/// The baud rate to use for Hairless MIDI.
constexpr unsigned long HAIRLESS_BAUD = 115200;

/// The maximum frame rate of the displays.
constexpr uint8_t MAX_FPS = 60;

/// Define the global instance `Control_Surface` as a true global variable.
/// Otherwise it is defined as a macro.
#define CS_TRUE_CONTROL_SURFACE_INSTANCE 1

/// Disable MIDI pipe support. As a consequence, you won't be able to use
/// multiple MIDI interfaces with Control_Surface, but you will save a couple
/// hundred bytes of RAM.
#define DISABLE_PIPES 0

// ========================================================================== //

END_CS_NAMESPACE