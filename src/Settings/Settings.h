#ifndef SETTINGS_H
#define SETTINGS_H


// #define DEBUG Serial // Print debug messages to Serial.

#define ANALOG_FILTER_SHIFT_FACTOR 4

#define BUTTON_DEBOUNCE_TIME 25 // The debounce time for momentary push buttons

// #define IGNORE_SYSEX // TODO: seems to fail

#define SYSEX_BUFFER_SIZE 128


// ---- Tests on PC ---- //
#ifndef ARDUINO
#ifdef DEBUG
#undef DEBUG
#define DEBUG cout
#endif
#endif

#endif // SETTINGS_H