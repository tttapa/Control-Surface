/**
 * This example demonstrates how to attach custom callbacks to Note or
 * Control Change MIDI Input Elements.
 *
 * @boards  AVR, AVR USB, Due, Nano 33, Teensy 3.x, ESP32
 *
 * Connections
 * -----------
 *
 * - Pin 2: The data pin of a Neopixel LED strip with (at least) 8 LEDs
 *
 * Behavior
 * --------
 *
 * If a MIDI Note On event for note 0x3C (C4 or middle C) is sent, its velocity
 * value sets the red value of the first LED, note 0x3D controls the green 
 * value of the first LED, note 0x3E controls the blue value of the first LED,
 * note 0x3F controls the red value of the second LED, 0x40 controls the green
 * value of the second LED, etc.
 *
 * Mapping
 * -------
 *
 * Route the MIDI output of a MIDI keyboard to the Arduino's MIDI input. Then
 * play some notes in the two octaves above middle C.  
 * Alternatively, map it in you DAW, for example, in VirtualDJ:  
 * `<color notered="0x3C" notegreen="0x3D" noteblue="0x3E" ... />`
 *
 * Written by PieterP, 2020-03-12  
 * https://github.com/tttapa/Control-Surface
 */

#include <FastLED.h>
#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

using namespace MIDI_Notes;

// Custom callback to handle incoming note events and control the LEDs
class NoteCCFastLEDCallbackRGB : public SimpleNoteCCValueCallback {
  public:
    NoteCCFastLEDCallbackRGB(CRGB *ledcolors)
        : ledcolors(ledcolors) {}

    // Called once upon initialization.
    void begin(const INoteCCValue &input) override { updateAll(input); }

    // Called each time a MIDI message is received and an LED has to be updated.
    // @param   input
    //          The NoteCCRange or NoteCCValue object this callback belongs to.
    //          This is the object that actually receives and stores the MIDI
    //          values.
    // @param   index
    //          The index of the value that changed. (zero-based)
    void update(const INoteCCValue &input, uint8_t index) override {
        // Get the MIDI value that changed [0, 127]
        uint8_t value      = input.getValue(index);
        uint8_t ledIndex   = index / 3;
        uint8_t colorIndex = index % 3;
        // Update the LED color
        ledcolors[ledIndex][colorIndex] = value;
    }

  private:
    // Pointer to array of FastLED color values for the LEDs
    CRGB *ledcolors;
};

// Create a type alias for the MIDI Note Input Element that uses
// the custom callback defined above.
template <uint8_t RangeLen>
using CustomNoteValueLED = GenericNoteCCRange<MIDIInputElementNote, 
                                              RangeLen, 
                                              NoteCCFastLEDCallbackRGB>;

// Define the array of leds.
Array<CRGB, 8> leds = {};
// The data pin with the strip connected.
constexpr uint8_t ledpin = 2;

// Create a MIDI input element that listens to all notes in the range C4 - B5
// (the range starts at C4 and has a length equal to `leds.length * 3` == 24).
// Note C4 is the red channel of the first LED,
// Note C#4 is the green channel of the first LED,
// Note D4 is the blue channel of the first LED,
// Note D#4 is the red channel of the second LED, etc.
CustomNoteValueLED<leds.length * 3> midiled = {note(C, 4), leds.data};

void setup() {
  // See FastLED examples and documentation for more information.
  FastLED.addLeds<NEOPIXEL, ledpin>(leds.data, leds.length);
  FastLED.setCorrection(TypicalPixelString);
  FastLED.setBrightness(128);
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
  FastLED.show();
}
