/**
 * This example demonstrates how to attach custom callbacks to Note or
 * Control Change MIDI Input Elements.
 *
 * @boards  AVR, AVR USB, Due, Nano 33, Teensy 3.x
 *
 * Connections
 * -----------
 *
 * - Pin 3 (PWM pin): An LED (with current-limiting resistor) to ground.
 *
 * Behavior
 * --------
 *
 * If a MIDI Note On event for note 0x3C (C4 or middle C) is sent, the built-in
 * LED will light up at full brightness,
 * if a Note Off event for that note is sent, the LED will light dimly.  
 * (A Note On event with a velocity of zero also counts as a Note Off event.)
 *
 * Mapping
 * -------
 *
 * Route the MIDI output of a MIDI keyboard to the Arduino's MIDI input. Then
 * play a middle C on the keyboard.
 *
 * Written by PieterP, 2020-03-10  
 * https://github.com/tttapa/Control-Surface
*/

#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

using namespace MIDI_Notes;

// Callback for Note or CC range or value input that turns on the LED at
// full brightness if the note or controller value is above the threshold,
// and turns on the LED at a lower brightness if the value is below the
// threshold.
class CustomLEDCallback : public SimpleNoteCCValueCallback {
 public:
  // Constructor
  CustomLEDCallback(pin_t ledPin, uint8_t lowBrightness)
    : ledPin(ledPin), lowBrightness(lowBrightness) {}

  // Called once upon initialization, set up the pin mode for the LED,
  // and output the initial value.
  void begin(const INoteCCValue &t) override {
    ExtIO::pinMode(ledPin, OUTPUT);
    updateAll(t);
  }

  // Called when the value of a note or controller value changes.
  // A MIDI Input Element can listen to a range of notes or
  // controllers, that's why the index of the note or controller
  // that changed is passed as a parameter.
  // In this example, only a single LED is used, so the index
  // doesn't really matter, it's used only to get the value of
  // the note/controller.
  void update(const INoteCCValue &t, uint8_t index) override {
    uint8_t value = t.getValue(index);
    bool state = value > threshold;
    state ? AH::ExtIO::digitalWrite(ledPin, HIGH)
          : AH::ExtIO::analogWrite(ledPin, lowBrightness);
  }

  // If you have an action in your custom callback that can be implemented more
  // efficiently when you need to update all values or outputs, you can override
  // the `updateAll` method.
  // It is called upon initialization (see the begin method) and when the bank
  // of a MIDI Input Element changes.
  // For example, turning off all LEDs in an LED strip can be done more
  // efficiently than turning off each LED individually.

 private:
  pin_t ledPin;
  uint8_t lowBrightness;
  const static uint8_t threshold = 0x3F;
};

// Create a type alias fore      the MIDI Note Input Element that uses
// the custom callback defined above.
using CustomNoteValueLED =
  GenericNoteCCRange<MIDIInputElementNote, 1, CustomLEDCallback>;

// Instantiate the LED that will light up when middle C is playing
CustomNoteValueLED led = {
  {note(C, 4), CHANNEL_1}, // Note C4 on MIDI channel 1
  {3, 10},                 // CustomLEDCallback constructor
  //│  └─── intensity when off
  //└────── pin with the LED connected (PWM capable)
};

void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface
}
