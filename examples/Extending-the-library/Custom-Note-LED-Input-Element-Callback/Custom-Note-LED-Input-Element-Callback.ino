/**
 * This example demonstrates how to attach custom actions to incoming Note or
 * Control Change MIDI events.
 *
 * @boards  AVR, AVR USB, Due, Nano 33 IoT, Nano 33 BLE, Teensy 3.x
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

// MIDI Input Element that listens for MIDI Note events that turns on the LED at
// full brightness if the note value (velocity) is above the threshold,
// and turns on the LED at a lower brightness if the value is below the
// threshold.

class CustomNoteLED
  // First, inherit from the MatchingMIDIInputElement base class. Indicate that
  // you want to listen to MIDI Note events by specifying the MIDI message type,
  // and use a simple 2-byte message matcher, since MIDI Note events have two
  // data bytes.
  : public MatchingMIDIInputElement<MIDIMessageType::NOTE_ON,
                                    TwoByteMIDIMatcher> {
 public:
  // Constructor
  CustomNoteLED(pin_t ledPin, MIDIAddress address, uint8_t lowBrightness)
    : MatchingMIDIInputElement(address), ledPin(ledPin),
      lowBrightness(lowBrightness) {}

  // Called once upon initialization, set up the pin mode for the LED,
  // and output the initial value.
  void begin() override {
    ExtIO::pinMode(ledPin, OUTPUT);
    ExtIO::analogWrite(ledPin, lowBrightness);
  }

  // Called when an incoming MIDI Note message matches this element's matcher
  // (i.e. it has the right MIDI address, channel and cable).
  // The match object that's passed as a parameter contains the velocity value
  // of the Note message that matched.
  void handleUpdate(TwoByteMIDIMatcher::Result match) override {
    if (match.value >= threshold)
      ExtIO::digitalWrite(ledPin, HIGH);
    else
      ExtIO::analogWrite(ledPin, lowBrightness);
  }

  // Called when the input element is reset (e.g. by an "All notes off" MIDI
  // event).
  void reset() override { ExtIO::analogWrite(ledPin, lowBrightness); }

 private:
  pin_t ledPin;
  uint8_t lowBrightness;
  const static uint8_t threshold = 0x01;
};

// Instantiate the LED that will light up when middle C is playing.
CustomNoteLED led {
  3,                           // Pin with the LED connected (PWM capable)
  {MIDI_Notes::C(4), CHANNEL_1}, // Note C4 on MIDI channel 1
  10,                          // Intensity when off
};

void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface
}
