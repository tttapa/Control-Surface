#include <Encoder.h> // Include the Encoder library.
// This must be done before the Control Surface library.
#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI interface to use for the Control Surface.
USBMIDI_Interface midi;

const pin_t dataPin = 11;  //Pin connected to DS of 74HC595
const pin_t clockPin = 13; //Pin connected to SH_CP of 74HC595
const pin_t latchPin = 10; //Pin connected to ST_CP of 74HC595

ShiftRegisterOut<8 * 3> sr(dataPin, clockPin, latchPin, MSBFIRST);
const uint8_t ShiftRegisterOutRGB::redBit = 2;
const uint8_t ShiftRegisterOutRGB::greenBit = 1;
const uint8_t ShiftRegisterOutRGB::blueBit = 0;

// Instantiate a transposer that transposes from -12 semitones to +12 semitones.
Transposer<-12, +12> transposer;

// Instantiate a button on pin 5 sending note C3 (0x3C) on channel 1
Bankable::NoteButton notebtn = {transposer, 5, {0x3C, CHANNEL_1}};

// Instantiate an encoder selector with the encoder wired to pins 2 and 3 and
// the switch pin connected to pin 4, with 4 pulses per step, without wrapping.
EncoderSelectorLEDs<transposer.N> sel = {
    transposer,
    {
        sr.red(0),
        sr.red(1),
        sr.red(2),
        sr.red(3),
        sr.red(4),
        sr.red(5),
        sr.red(6),
        sr.red(7),
    },
    {
        2,
        3,
        4,
    },
    4,
    Wrap::NoWrap,
};

void setup() {
    Control_Surface.begin(); // Initialize Control Surface
}

void loop() { Control_Surface.loop(); }
