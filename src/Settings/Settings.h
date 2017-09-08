
// #define DEBUG // Print debug messages to Serial. (Not compatible with AVR, because it uses Serial.printf)

#define SINGLE_BYTE_AVERAGE // Save only the 8 most significant bits of the analog values in the Analog average buffer instead of 10. Only takes up half the memory.

// #define PITCH_BEND_RESOLUTION_14_BITS // use true 14-bit resolution instead of 10-bit resolution padded with four zeros. (only on boards that support it, e.g. Teensy and Arduino Due)