#include "USBMidi.h"

const char *MIDI_STATUS_NAMES[] = {
    "Note Off\t",
    "Note On\t\t",
    "Key Pressure\t",
    "Control Change\t",
    "Program Change\t",
    "Channel Pressure",
    "Pitch Bend\t",
    "Unknown\t"};

USBMidi::USBMidi() {} // Constructor

USBMidi::~USBMidi() // Deconstructor
{
  if (debug)
    Serial.end();
#if !(defined(USBCON) || defined(CORE_TEENSY)) // If the main MCU doesn't have a USB connection
  else
    Serial.end();
#endif
  if (blinkEnabled)
    pinMode(blinkPin, INPUT);
}

void USBMidi::begin(unsigned long baud, bool debug) // If the main MCU doesn't have a USB connection or if debug mode is enabled, start the Serial connection at the desired baud rate
{
  if (debug)
  {
    this->debug = true;
    Serial.begin(baud);
    Serial.println("MIDI DEBUG MODE");
  }
#if !(defined(USBCON) || defined(CORE_TEENSY)) // If the main MCU doesn't have a USB connection
  else
  {
    Serial.begin(baud); // Start serial communication with ATmega16U2
  }
#endif
}

void USBMidi::send(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2) // Send a 3-uint8_t MIDI packet over USB (or Serial or debug Serial)
{
  c--; // Channels are zero-based

  m &= 0xF0;       // bitmask high nibble
  m |= 0b10000000; // set msb
  c &= 0xF;        // bitmask low nibble
  d1 &= 0x7F;      // clear msb
  d2 &= 0x7F;      // clear msb

  if (blinkEnabled)
    digitalWrite(blinkPin, HIGH);

  if (debug)
    sendMIDIDebug(m, c, d1, d2);
  else
    sendMIDIUSB(m, c, d1, d2);

  if (blinkDelay != 0)
    delay(blinkDelay);
  if (blinkEnabled)
    digitalWrite(blinkPin, LOW);
}

void USBMidi::send(uint8_t m, uint8_t c, uint8_t d1) // Send a 2-uint8_t MIDI packet over USB (or Serial or debug Serial)
{
  c--; // Channels are zero-based

  m &= 0xF0;       // bitmask high nibble
  m |= 0b10000000; // set msb
  c &= 0xF;        // bitmask low nibble
  d1 &= 0x7F;      // clear msb

  if (blinkEnabled)
    digitalWrite(blinkPin, HIGH);

  if (debug)
    sendMIDIDebug(m, c, d1);
  else
    sendMIDIUSB(m, c, d1);

  if (blinkDelay != 0)
    delay(blinkDelay);
  if (blinkEnabled)
    digitalWrite(blinkPin, LOW);
}

void USBMidi::setDelay(int d) // d = delay in milliseconds after each MIDI message
{
  blinkDelay = d;
}

void USBMidi::blink(uint8_t p) // p = pin with an LED connected to blink on each MIDI message
{
  blinkPin = p;
  pinMode(blinkPin, OUTPUT);
  blinkEnabled = true;
}

void USBMidi::noBlink()
{
  if (blinkEnabled)
  {
    pinMode(blinkPin, INPUT);
    blinkEnabled = false;
  }
}

void USBMidi::sendMIDIUSB(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2) // Send a 3-byte MIDI packet over USB or Serial (as a binary MIDI packet)
{
#if defined(CORE_TEENSY) // If it's a Teensy board
  usb_midi_write_packed((m >> 4) | ((m | c) << 8) | (d1 << 16) | (d2 << 24));
#elif defined(USBCON) // If the main MCU has a USB connection but is not a Teensy
  midiEventPacket_t msg = {m >> 4, m | c, d1, d2};
  MidiUSB.sendMIDI(msg);
  MidiUSB.flush();
#else                 // If the main MCU doesn't have a USB connection
  Serial.write(m | c); // Send the MIDI message over Serial.
  Serial.write(d1);
  Serial.write(d2);
  Serial.flush();
#endif
}

void USBMidi::sendMIDIUSB(uint8_t m, uint8_t c, uint8_t d1) // Send a 2-byte MIDI packet over USB or Serial (as a binary MIDI packet)
{
#if defined(CORE_TEENSY) // If it's a Teensy board
  usb_midi_write_packed((m >> 4) | ((m | c) << 8) | (d1 << 16));
#elif defined(USBCON) // If the main MCU has a USB connection but is not a Teensy
  midiEventPacket_t msg = {m >> 4, m | c, d1, 0};
  MidiUSB.sendMIDI(msg);
  MidiUSB.flush();
#else                 // If the main MCU doesn't have a USB connection
  Serial.write(m | c); // Send the MIDI message over Serial.
  Serial.write(d);
  Serial.flush();
#endif
}

void USBMidi::sendMIDIDebug(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2) // Print debug information about a 3-byte MIDI event to the Serial port
{
  Serial.print(MIDI_STATUS_NAMES[min((m >> 4) - 8, 7)]);
  Serial.print("\tChannel: ");
  Serial.print(c + 1);
  Serial.print("\tData 1: ");
  Serial.print(d1, HEX);
  Serial.print("\tData 2: ");
  Serial.print(d2, HEX);
  Serial.print("\r\n");
  // Serial.print((unsigned long)(m >> 4) | ((m | c) << 8) | (d1 << 16) | (d2 << 24), HEX);
  Serial.flush();
}

void USBMidi::sendMIDIDebug(uint8_t m, uint8_t c, uint8_t d1) // Print debug information about a 2-byte MIDI event to the Serial port
{
  Serial.print(MIDI_STATUS_NAMES[min((m >> 4) - 8, 7)]);
  Serial.print("\tChannel: ");
  Serial.print(c + 1);
  Serial.print("\tData 1: ");
  Serial.print(d1);
  Serial.print("\r\n");
  // Serial.print((unsigned long)(m >> 4) | ((m | c) << 8) | (d1 << 16), HEX);
  Serial.flush();
}

USBMidi USBMidiController;
