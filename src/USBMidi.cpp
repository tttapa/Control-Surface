#include "Arduino.h"
#include "USBMidi.h"

#if defined(USBCON) && !defined(CORE_TEENSY)  // If the main MCU has a USB connection but is not a Teensy
#include "MIDIUSB.h"
#endif

const char *MIDI_STATUS_NAMES[] = {
    "Note Off\t",
    "Note On\t\t",
    "Key Pressure\t",
    "Control Change\t",
    "Program Change\t",
    "Channel Pressure",
    "Pitch Bend\t",
    "Unknown\t"};

USBMidi::USBMidi() {}

void USBMidi::begin(unsigned long baud, bool debug)
{
  if (debug)
  {
    _debug = true;
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

USBMidi::~USBMidi()
{
  if (_debug)
    Serial.end();
#if !(defined(USBCON) || defined(CORE_TEENSY)) // If the main MCU doesn't have a USB connection
  Serial.end();
#endif
  if (_blinkEn)
    pinMode(_blinkPin, INPUT);
}

void USBMidi::send(byte m, byte c, byte d1, byte d2)
{
  c--; // Channels are zero-based

  m &= 0xF0;
  m |= 0b10000000;
  c &= 0xF;
  d1 &= 0x7F;
  d2 &= 0x7F;

  if (_blinkEn)
    digitalWrite(_blinkPin, HIGH);

  if (_debug)
  {
    Serial.print(MIDI_STATUS_NAMES[min((m >> 4) - 8, 7)]);
    Serial.print("\tChannel: ");
    Serial.print(c + 1);
    Serial.print("\tData 1: ");
    Serial.print(d1, HEX);
    Serial.print("\tData 2: ");
    Serial.print(d2, HEX);
    Serial.print("\r\n");
    //Serial.print((unsigned long) (m>>4) & 0xF) | (((m | c) & 0xFF) << 8) | ((d1 & 0x7F) << 16) | ((d2 & 0x7F) << 24), HEX);
    Serial.flush();
  }
  else // no debug
  {
#if defined(CORE_TEENSY)  // If it's a Teensy board
    usb_midi_write_packed((m >> 4) | ((m | c) << 8) | (d1 << 16) | (d2 << 24));
#elif defined(USBCON)     // If the main MCU has a USB connection but is not a Teensy
    midiEventPacket_t msg = {m >> 4, m | c, d1, d2};
    MidiUSB.sendMIDI(msg);
    MidiUSB.flush();
#else                     // If the main MCU doesn't have a USB connection
    Serial.write(m | c);  // Send the MIDI message over Serial.
    Serial.write(d1);
    Serial.write(d2);
    Serial.flush();
#endif
  } // end no debug

  if (_blinkDelay != 0)
    delay(_blinkDelay);
  if (_blinkEn)
    digitalWrite(_blinkPin, LOW);
}

void USBMidi::send(byte m, byte c, byte d1)
{
  c--; // Channels are zero-based

  m &= 0xF0;
  m |= 0b10000000;
  c &= 0xF;
  d1 &= 0x7F;

  if (_blinkEn)
    digitalWrite(_blinkPin, HIGH);

  if (_debug)
  {
    Serial.print(MIDI_STATUS_NAMES[min((m >> 4) - 8, 7)]);
    Serial.print("\tChannel: ");
    Serial.print((c & 0xF) + 1);
    Serial.print("\tData 1: ");
    Serial.print(d1 & 0x7F);
    Serial.print("\r\n");
    //Serial.print((unsigned long) ((m>>4) & 0xF) | (((m | c) & 0xFF) << 8) | ((d1 & 0x7F) << 16), HEX);
    Serial.flush();
  }
  else // no debug
  {
#if defined(CORE_TEENSY)  // If it's a Teensy board
    usb_midi_write_packed((m >> 4) | ((m | c) << 8) | (d1 << 16));
#elif defined(USBCON)     // If the main MCU has a USB connection but is not a Teensy
    midiEventPacket_t msg = {m >> 4, m | c, d1, 0};
    MidiUSB.sendMIDI(msg);
    MidiUSB.flush();
#else                     // If the main MCU doesn't have a USB connection
    Serial.write(m | c);  // Send the MIDI message over Serial.
    Serial.write(d);
    Serial.flush();
#endif
  } // end no debug

  if (_blinkDelay != 0)
    delay(_blinkDelay);
  if (_blinkEn)
    digitalWrite(_blinkPin, LOW);
}

void USBMidi::setDelay(int d)  // d = delay in milliseconds after each MIDI message
{
  _blinkDelay = d;
}

void USBMidi::blink(byte p)  // p = pin with an LED connected to blink on each MIDI message
{
  _blinkPin = p;
  pinMode(_blinkPin, OUTPUT);
  _blinkEn = true;
}

void USBMidi::noBlink()
{
  if (_blinkEn)
  {
    pinMode(_blinkPin, INPUT);
    _blinkEn = false;
  }
}

USBMidi USBMidiController;
