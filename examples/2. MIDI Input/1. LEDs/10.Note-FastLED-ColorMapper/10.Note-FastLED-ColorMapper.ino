/**
 * This example demonstrates the use of addressable LEDs that respond to 
 * incoming  MIDI note events. This example uses a custom color mapper to
 * get a rainbow effect across the LED strip.
 * 
 * @boards  AVR, AVR USB, Due, Nano 33 IoT, Teensy 3.x, ESP32
 * 
 * @note    You might lose incoming MIDI data while the LED strip is being 
 *          updated. To avoid this, don't use an Arduino UNO.  
 *          See <https://github.com/FastLED/FastLED/wiki/Interrupt-problems>
 * 
 * Connections  
 * -----------
 * 
 * - 2: Data pin of NeoPixel LED strip with at least 8 pixels.
 * 
 * Behavior
 * --------
 * 
 * If a MIDI Note On event for note 0x3C (C4 or middle C) is sent, the first LED
 * will light up, if a Note Off event for that note is sent, the LED will turn 
 * off.  
 * If a MIDI Note On event for note 0x3D (C#4) is sent, the second LED will 
 * light up, etc.  
 * (A Note On event with a velocity of zero also counts as a Note Off event.)
 * 
 * Mapping
 * -------
 * 
 * Route the MIDI output of a MIDI keyboard to the Arduino's MIDI input. Then
 * play a middle C and some notes above it on the keyboard.
 * 
 * Written by PieterP, 2019-10-15  
 * https://github.com/tttapa/Control-Surface
 */
#include <FastLED.h>
// Must be before Control Surface to enable FastLED features of Control Surface
#include <Control_Surface.h>

// Define the array of leds.
Array<CRGB, 8> leds {};
// The data pin with the strip connected.
constexpr uint8_t ledpin = 2;

USBMIDI_Interface midi;

// Create a functor that maps the velocity and the index of a note to a color.
struct RainbowColorMapper {
  CHSV operator()(uint8_t velocity, uint8_t index) const {
    return CHSV(255 * index / leds.length, 255, 255u * velocity / 127u);
  }
};

NoteRangeFastLED<leds.length, RainbowColorMapper> midiled {
  leds,
  MIDI_Notes::C[4],
};

void setup() {
  // See FastLED examples and documentation for more information.
  FastLED.addLeds<NEOPIXEL, ledpin>(leds.data, leds.length);
  FastLED.setCorrection(TypicalPixelString);
  midiled.setBrightness(128);
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
  if (midiled.getDirty()) { // If the colors changed
    FastLED.show();         // Update the LEDs with the new colors
    midiled.clearDirty();   // Clear the dirty flag
  }
}
