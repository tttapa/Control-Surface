#include <Encoder.h> // Include the Encoder library.
// This must be done before the Control Surface library.
#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI interface to use for the Control Surface.
USBMIDI_Interface midi;

const pin_t dataPin = 11;  //Pin connected to DS of 74HC595
const pin_t clockPin = 13; //Pin connected to SH_CP of 74HC595
const pin_t latchPin = 10; //Pin connected to ST_CP of 74HC595

ShiftRegisterOut sr(dataPin, clockPin, latchPin, MSBFIRST, 8 * 3);
const uint8_t ShiftRegisterOut::redBit = 2;
const uint8_t ShiftRegisterOut::greenBit = 1;
const uint8_t ShiftRegisterOut::blueBit = 0;

// Instantiate a transposer that transposes from -12 semitones to +12 semitones.
Transposer<-4, +3> transposer;

// Instantiate a button on pin 5 sending note C3 (0x3C) on channel 1
Bankable::NoteButton note = {transposer, 5, 0x3C, 1};

// Instantiate an encoder selector with the encoder wired to pins 2 and 3 and
// the switch pin connected to pin 4, with 4 pulses per step, without wrapping.
EncoderSelectorLEDs<transposer.N> sel = {
    transposer,
    generateArray<pin_t, transposer.N>(sr.red(0), 3),
    {2, 3, 4},
    4,
    false
};

void setup() {
    Control_Surface.begin(); // Initialize Control Surface
}

void loop() { Control_Surface.loop(); }
