/**
 * This example demonstrates how to attach custom callbacks to Note or
 * Control Change MIDI Input Elements.
 *
 * @boards  AVR, AVR USB, Due, Nano 33 IoT, Teensy 3.x, ESP32
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
// Include FastLED before Control Surface
#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// Custom MIDI Input Element to handle incoming note events and control the LEDs.
template <uint8_t RangeLen>
class CustomNoteLED : public MatchingMIDIInputElement<MIDIMessageType::NOTE_ON,
                                                      TwoByteRangeMIDIMatcher> {
 public:
  // Constructor
  CustomNoteLED(CRGB *ledcolors, MIDIAddress address)
    : MatchingMIDIInputElement<MIDIMessageType::NOTE_ON,
                               TwoByteRangeMIDIMatcher>({address, RangeLen}),
      ledcolors(ledcolors) {}

  // Called once upon initialization.
  void begin() override {}

  // Called when an incoming MIDI Note message matches this element's matcher
  // (i.e. it has the right MIDI address, channel and cable).
  // The match object that's passed as a parameter contains the velocity value
  // of the Note message that matched.
  void handleUpdate(typename TwoByteRangeMIDIMatcher::Result match) override {
    updateLED(match.index, match.value);
  }

  // Called each time a MIDI message is received and an LED has to be updated.
  void updateLED(uint8_t index, uint8_t value) {
    uint8_t ledIndex = index / 3;
    uint8_t colorIndex = index % 3; // 0 = R, 1 = G, 2 = B
    // Update the LED color
    ledcolors[ledIndex][colorIndex] = value;
    dirty = true;
  }

  // Check if the colors changed since the last time the dirty flag was cleared.
  bool getDirty() const { return dirty; }
  // Clear the dirty flag.
  void clearDirty() { dirty = false; }

 private:
  // Pointer to array of FastLED color values for the LEDs
  CRGB *ledcolors;
  // Should the LEDs be updated by FastLED?
  bool dirty = true;
};

// Define the array of leds.
Array<CRGB, 8> leds {};
// The data pin with the strip connected.
constexpr uint8_t ledpin = 2;

// Create a MIDI input element that listens to all notes in the range C4 - B5
// (the range starts at C4 and has a length equal to `leds.length * 3` == 24).
// Note C4 is the red channel of the first LED,
// Note C#4 is the green channel of the first LED,
// Note D4 is the blue channel of the first LED,
// Note D#4 is the red channel of the second LED, etc.
CustomNoteLED<leds.length * 3> midiled {leds.data, MIDI_Notes::C(4)};

void setup() {
  // See FastLED examples and documentation for more information.
  FastLED.addLeds<NEOPIXEL, ledpin>(leds.data, leds.length);
  FastLED.setCorrection(TypicalPixelString);
  FastLED.setBrightness(128);
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
  if (midiled.getDirty()) {
    FastLED.show();
    midiled.clearDirty();
  }
}
