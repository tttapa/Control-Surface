#include <Control_Surface.h> // Include the Control Surface library

USBMIDI_Interface midi;

const pin_t dataPin  = 12;  // Pin connected to DS of 74HC595
const pin_t clockPin = 13;  // Pin connected to SH_CP of 74HC595
const pin_t latchPin = 10;  // Pin connected to ST_CP of 74HC595

// Create a new shift register output connected to pins 12, 13 and 10,
// shift the data out with the most significant bit first.
// There are 8 outputs in total.
ShiftRegisterOut<8> sr(dataPin, clockPin, latchPin, MSBFIRST);

uint8_t track = 1;
Channel channel = CHANNEL_1;
// Note: a VU meter of 12 LEDs will give the best effect, because
// in the MCU protocol, there are 12 values
MCU::VULEDs<8> vu = { 
  {track, channel}, 
  {{
    sr.pin(0),
    sr.pin(1),
    sr.pin(2),
    sr.pin(3),
    sr.pin(4),
    sr.pin(5),
    sr.pin(6),
    sr.pin(7),
  }},
  150, // decay time in milliseconds or MCU::VU::NO_DECAY
};

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();  
}
