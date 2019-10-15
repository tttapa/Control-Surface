/**
 * This example demonstrates the use of addressable LEDs that respond to 
 * incoming  MIDI note events.
 * 
 * @note    The FastLED API will change in the near future.
 * 
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 * 
 * Connections  
 * -----------
 * 
 * Connect an WS2801 LED to pin 2.
 * 
 * Behavior
 * --------
 * 
 * If a MIDI Note On event for note 0x3C (C4 or middle C) is sent, the LED will
 * light up, if a Note Off event for that note is sent, the LED will turn off.  
 * (A Note On event with a velocity of zero also counts as a Note Off event.)
 * 
 * Mapping
 * -------
 * 
 * Route the MIDI output of a MIDI keyboard to the Arduino's MIDI input. Then
 * play a middle C on the keyboard.
 * 
 * Written by PieterP, 2019-10-15  
 * https://github.com/tttapa/Control-Surface
 */
#include <FastLED.h>
// Must be before Control Surface
#include <Control_Surface.h>

// Define the array of leds
CRGB led = CRGB::Black;
constexpr uint8_t ledpin = 2;

USBMIDI_Interface midi;

using namespace MIDI_Notes;
NoteValueFastLED<> midiled = {note(C, 4), &led};

void setup() {
  FastLED.addLeds<NEOPIXEL, ledpin>(&led, 1);
  Control_Surface.begin();
  midiled.callback.setBrightness(64);
}

void loop() { 
  Control_Surface.loop();
  FastLED.show();
}