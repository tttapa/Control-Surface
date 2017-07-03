#include "Arduino.h"
#include "USBMidi.h"

#if defined(USBCON) && !defined(DEBUG)
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
#if !(defined(USBCON) || defined(CORE_TEENSY)) // If you're compiling for an Arduino that has no USB connection in the main MCU
  else
  {
    Serial.begin(baud); // Start communication with ATmega16U2 @31250 baud (for MIDI firmware: Hiduino: https://github.com/ddiakopoulos/hiduino)
  }
#endif
}

USBMidi::~USBMidi()
{
  if (_debug)
    Serial.end();
#if !(defined(USBCON) || defined(CORE_TEENSY)) // If you're compiling for an Arduino that has USB connection in the main MCU
  Serial.end();
#endif
  if (_blinkEn)
  {
    pinMode(_blinkPin, INPUT);
    _blinkEn = false;
  }
}

void USBMidi::send(byte m, byte c, byte d1, byte d2)
{
  c--; // Channels are zero-based

  if (_blinkEn)
    digitalWrite(_blinkPin, HIGH);

  if (_debug)
  {
    Serial.print(MIDI_STATUS_NAMES[min((m >> 4) - 8, 7)]);
    Serial.print("\tChannel: ");
    Serial.print((c & 0xF) + 1);
    Serial.print("\tData 1: ");
    Serial.print(d1 & 0x7F);
    Serial.print("\tData 2: ");
    Serial.print(d2 & 0x7F);
    Serial.print("\r\n");
    //Serial.print((unsigned long) (m>>4) & 0xF) | (((m | c) & 0xFF) << 8) | ((d1 & 0x7F) << 16) | ((d2 & 0x7F) << 24), HEX);
  }
  else
  {
#if defined(CORE_TEENSY) //only include these lines when compiling for a Teensy board
    usb_midi_write_packed(((m >> 4) & 0xF) | (((m | c) & 0xFF) << 8) | ((d1 & 0x7F) << 16) | ((d2 & 0x7F) << 24));
#elif defined(USBCON) //only include these lines when compiling for an Arduino if you're compiling for an Arduino that has USB connection in the main MCU but is not a Teensy
    midiEventPacket_t msg = {(m >> 4) & 0xF, (m | c) & 0xFF, d1 & 0x7F, d2 & 0x7F};
    MidiUSB.sendMIDI(msg);
    MidiUSB.flush();
#else                 // If you're compiling for an Arduino that has no USB connection in the main MCU
    //* The format of the message to send via serial. We create a new data structure, that can store 3 bytes at once.  This will be easier to send as MIDI. */
    typedef struct
    {
      unsigned int channel : 4; // second nibble : midi channel (0-15) (channel and status are swapped, because Arduino is Little Endian)
      unsigned int status : 4;  // first  nibble : status message (NOTE_ON, NOTE_OFF or CC (controlchange)
      uint8_t data1;            // second byte   : first value (0-127), controller number or note number
      uint8_t data2;            // third  byte   : second value (0-127), controller value or velocity
    } t_midiMsg;                // We call this structure 't_midiMsg'

    t_midiMsg msg;
    msg.status = (m >> 4) & 0xF;
    msg.channel = c & 0xF;
    msg.data1 = d1 & 0x7F;
    msg.data2 = d2 & 0x7F;
    Serial.write((uint8_t *)&msg, sizeof(msg)); // Send the MIDI message.
#endif
  }

  if (_blinkDelay != 0)
    delay(_blinkDelay);
  if (_blinkEn)
    digitalWrite(_blinkPin, LOW);
}

void USBMidi::send(byte m, byte c, byte d1)
{
  c--; // Channels are zero-based

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
  }
  else
  {
#if defined(CORE_TEENSY) //only include these lines when compiling for a Teensy board
    usb_midi_write_packed(((m >> 4) & 0xF) | (((m | c) & 0xFF) << 8) | ((d1 & 0x7F) << 16));
#elif defined(USBCON) //only include these lines when compiling for an Arduino if you're compiling for an Arduino that has USB connection in the main MCU but is not a Teensy
    midiEventPacket_t msg = {(m >> 4) & 0xF, (m | c) & 0xFF, d1 & 0x7F, 0};
    MidiUSB.sendMIDI(msg);
    MidiUSB.flush();
#else                 // If you're compiling for an Arduino that has no USB connection in the main MCU
    //* The format of the message to send via serial. We create a new data structure, that can store 2 bytes at once.  This will be easier to send as MIDI. */
    typedef struct
    {
      unsigned int channel : 4; // second nibble : midi channel (0-15) (channel and status are swapped, because Arduino is Little Endian)
      unsigned int status : 4;  // first  nibble : status message
      uint8_t data;             // second byte   : data  (0-127)
    } t_midiMsg;                // We call this structure 't_midiMsg'

    t_midiMsg msg;
    msg.status = (m >> 4) & 0xF;
    msg.channel = c & 0xF;
    msg.data = d1 & 0x7F;
    Serial.write((uint8_t *)&msg, sizeof(msg)); // Send the MIDI message.
#endif
  }
  if (_blinkDelay != 0)
    delay(_blinkDelay);
  if (_blinkEn)
    digitalWrite(_blinkPin, LOW);
}

void USBMidi::setDelay(int d)
{
  _blinkDelay = d;
}

void USBMidi::blink(byte p)
{ // pin
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
