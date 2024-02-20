/**
 * This example demonstrates how to create a custom MIDI Input Element similar
 * to the NoteLED class.
 *
 * @boards  AVR, AVR USB, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Teensy 3.x
 *
 * Connections
 * -----------
 *
 * - None (built-in LED is used)
 *
 * Behavior
 * --------
 *
 * If a MIDI Note On event for note 0x3C (C4 or middle C) is sent, the built-in
 * LED will light up,
 * if a Note Off event for that note is sent, the LED will turn off.  
 * (A Note On event with a velocity of zero also counts as a Note Off event.)
 *
 * Mapping
 * -------
 *
 * Route the MIDI output of a MIDI keyboard to the Arduino's MIDI input. Then
 * play a middle C on the keyboard.
 *
 * Alternatively, replace @ref USBMIDI_Interface by @ref USBDebugMIDI_Interface
 * and send `90 3C 7F` and/or `80 3C 7F` in the Serial monitor.
 *
 * Written by PieterP, 2024-02-20  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// MIDI Input Element that listens for MIDI Note events and that turns on the
// LED if the note value (velocity) is above the threshold, and turns off the
// LED if the value is below the threshold.

class CustomNoteLED
  // First, inherit from the MatchingMIDIInputElement base class. Indicate that
  // you want to listen to MIDI Note events by specifying the MIDI message type,
  // and use a simple 2-byte message matcher, since MIDI Note events have two
  // data bytes.
  : public MatchingMIDIInputElement<MIDIMessageType::NoteOn,
                                    TwoByteMIDIMatcher> {
 public:
  // Constructor
  CustomNoteLED(pin_t ledPin, MIDIAddress address)
    : MatchingMIDIInputElement(address), ledPin(ledPin) {}

  // Called once upon initialization, set up the pin mode for the LED.
  void begin() override { ExtIO::pinMode(ledPin, OUTPUT); }

  // Called when an incoming MIDI Note message matches this element's matcher
  // (i.e. it has the right MIDI address, channel and cable).
  // The match object that's passed as a parameter contains the velocity value
  // of the Note message that matched.
  void handleUpdate(TwoByteMIDIMatcher::Result match) override {
    ExtIO::digitalWrite(ledPin, match.value >= threshold ? HIGH : LOW);
  }

  // Called when the input element is reset (e.g. by an "All notes off" MIDI
  // event).
  void reset() override { ExtIO::digitalWrite(ledPin, LOW); }

 private:
  pin_t ledPin;
  const static uint8_t threshold = 0x01;
};

// Instantiate the LED that will light up when middle C is playing.
CustomNoteLED led {
  LED_BUILTIN,                   // Pin with the LED connected
  {MIDI_Notes::C[4], Channel_1}, // Note C4 on MIDI channel 1
};

void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface
}
