#ifndef SETTINGSWRAPPER_H
#error "Do not include this file directly, use the wrapper !"
#endif

// ---------------------------- Debug Settings --------------------------- //
// ======================================================================= //

/* Print all debug messages */
// #define DEBUG

/* Print all incoming MIDI messages */
// #define DEBUG_MIDI_PACKETS

/* Print timing/profiling information */
// #define DEBUG_TIME

/* The default debug output */
#define DEBUG_OUT Serial1

// ---------------------------- User Settings ---------------------------- //
// ======================================================================= //

/* The factor for the analog filter: 
   Difference equation: y[n] = k*x[n] + (1-k)*y[n-1]  
   where k = 0.5 ^ ANALOG_FILTER_SHIFT_FACTOR */
#define ANALOG_FILTER_SHIFT_FACTOR 4

/* The debounce time for momentary push buttons in milliseconds */
#define BUTTON_DEBOUNCE_TIME 25

/* Don't parse System Exclusive messages (TODO) */
// #define IGNORE_SYSEX

/* The length of the maximum System Exclusive message 
   that can be received. The maximum length sent by
   the MCU protocol is 120 bytes. */
#define SYSEX_BUFFER_SIZE 128

/* The baud rate to use for Hairless MIDI */
#define HAIRLESS_BAUD 115200

// ======================================================================= //