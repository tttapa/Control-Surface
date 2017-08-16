#ifndef USBMidi_h_
#define USBMidi_h_

#include "Arduino.h"

#if defined(USBCON) && !defined(CORE_TEENSY) // If the main MCU has a USB connection but is not a Teensy
#include "MIDIUSB.h"
#endif

#if defined(CORE_TEENSY) && !(defined(USB_MIDI_AUDIO_SERIAL) || defined(USB_MIDI) || defined(USB_MIDI_SERIAL))
#error Please select a MIDI option in the 'Tools > USB Type' menu.
#endif

#define MIDI_BAUD 31250 // MIDI hardware baud rate (for use with HIDUINO or 5 pin DIN MIDI connector)

const uint8_t NOTE_OFF = 0x80;
const uint8_t NOTE_ON = 0x90;
const uint8_t KEY_PRESSURE = 0xA0;
const uint8_t CC = 0xB0;
const uint8_t PROGRAM_CHANGE = 0xC0;
const uint8_t CHANNEL_PRESSURE = 0xD0;
const uint8_t PITCH_BEND = 0xE0;

class USBMidi
{
private:
  bool blinkEnabled = false;
  uint8_t blinkPin;
  int blinkDelay = 0;
  bool debug = false;

  void sendMIDIUSB(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2);   // Send a 3-byte MIDI packet over USB or Serial (as a binary MIDI packet)
  void sendMIDIUSB(uint8_t m, uint8_t c, uint8_t d1);               // Send a 2-byte MIDI packet over USB or Serial (as a binary MIDI packet)
  void sendMIDIDebug(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2); // Print debug information about a 3-byte MIDI event to the Serial port
  void sendMIDIDebug(uint8_t m, uint8_t c, uint8_t d1);             // Print debug information about a 2-byte MIDI event to the Serial port

public:
  USBMidi();                                                      // Constructor
  ~USBMidi();                                                     // Destructor
  void begin(unsigned long baud = MIDI_BAUD, bool debug = false); // If the main MCU doesn't have a USB connection or if debug mode is enabled, start the Serial connection at the desired baud rate
  void send(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2);        // Send a 3-byte MIDI packet over USB (or Serial or debug Serial)
  void send(uint8_t m, uint8_t c, uint8_t d1);                    // Send a 2-byte MIDI packet over USB (or Serial or debug Serial)
  void setDelay(int blinkDelay);                                  // Set a delay between messages to prevent flooding the connection
  void blink(uint8_t blinkPin);                                   // Blink an LED on the specified pin on each MIDI message
  void noBlink();                                                 // Stop blinking the LED on each MIDI message
};

extern USBMidi USBMidiController;
#endif
