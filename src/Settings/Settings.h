
// #define DEBUG // Print debug messages to Serial. (Not compatible with AVR, because it uses Serial.printf)

#define ANALOG_AVERAGE 8 // Use the average of 8 samples for the Analog class
#define ANALOG_HI_RES_AVERAGE 16 // Use the average of 8 samples for the AnalogHiRes class
#define SINGLE_BYTE_AVERAGE // Save only the 8 most significant bits of the analog values in the Analog average buffer instead of 10. Only takes up half the memory.

#define BUTTON_DEBOUNCE_TIME 25 // The debounce time for momentary push buttons

// #define IGNORE_SYSEX

// #define NO_MIDI_INPUT

#define SINGLE_BYTE_AVERAGE

// #define PITCH_BEND_RESOLUTION_14_BITS // use true 14-bit resolution instead of 10-bit resolution padded with four zeros. (only on boards that support it, e.g. Teensy and Arduino Due)

#define MIDI_BUFFER_SIZE 128

;
