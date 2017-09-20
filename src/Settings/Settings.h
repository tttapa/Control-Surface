
// #define DEBUG // Print debug messages to Serial. (Not compatible with AVR, because it uses Serial.printf)

#define ANALOG_AVERAGE 8 // Use the average of 8 samples for the Analog class
#define ANALOG_HI_RES_AVERAGE 16 // Use the average of 8 samples for the AnalogHiRes class
#define SINGLE_BYTE_AVERAGE // Save only the 8 most significant bits of the analog values in the Analog average buffer instead of 10. Only takes up half the memory.