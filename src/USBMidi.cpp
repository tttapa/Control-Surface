#include "Arduino.h"
#include "USBMidi.h"

#if defined (USBCON)
#include "MIDIUSB.h"
#endif

USBMidi::USBMidi(void) {
#if ! (defined(USBCON) || defined (CORE_TEENSY)) // If you're compiling for an Arduino that has USB connection in the main MCU
  Serial.begin(MIDI_BAUD); // Start communication with ATmega16U2 @31250 baud (for MIDI firmware: Hiduino: https://github.com/ddiakopoulos/hiduino)
#endif 
}

USBMidi::~USBMidi(void) {
#if ! (defined(USBCON) || defined (CORE_TEENSY)) // If you're compiling for an Arduino that has USB connection in the main MCU
   Serial.end();
#endif 
  if(blinkEn) {
    pinMode(blinkPin, INPUT);
    blinkEn = false;
  }   
}

USBMidi USB;

void USBMidi::send(byte m, byte c, byte d1, byte d2) {
  c--; // Channels are zero-based

#if defined (CORE_TEENSY)  //only include these lines when compiling for a Teensy board
  usb_midi_write_packed(((m>>4) & 0xF) | (((m | c) & 0xFF) << 8) | ((d1 & 0x7F) << 16) | ((d2 & 0x7F) << 24));
#elif defined(USBCON)  //only include these lines when compiling for an Arduino if you're compiling for an Arduino that has USB connection in the main MCU but is not a Teensy
  midiEventPacket_t msg = {(m>>4)&0xF, (m | c)&0xFF, d1&0x7F, d2&0x7F};
  MidiUSB.sendMIDI(msg);
  MidiUSB.flush();
#else // If you're compiling for an Arduino that has no USB connection in the main MCU
  //* The format of the message to send via serial. We create a new data structure, that can store 3 bytes at once.  This will be easier to send as MIDI. */
  typedef struct {
    unsigned int channel : 4;   // second nibble : midi channel (0-15) (channel and status are swapped, because Arduino is Little Endian)
    unsigned int status : 4;    // first  nibble : status message (NOTE_ON, NOTE_OFF or CC (controlchange) 
    uint8_t data1;              // second byte   : first value (0-127), controller number or note number
    uint8_t data2;              // third  byte   : second value (0-127), controller value or velocity
  } t_midiMsg;          // We call this structure 't_midiMsg'

  t_midiMsg msg;
  msg.status = (m>>4) & 0xF;
  msg.channel = c & 0xF;
  msg.data1 = d1 & 0x7F;
  msg.data2 = d2 & 0x7F;
  Serial.write((uint8_t *)&msg, sizeof(msg));  // Send the MIDI message.
#endif
}

void USBMidi::send(byte m, byte c, byte d1) {
  c--; // Channels are zero-based

  if (blinkEn)
    digitalWrite(blinkPin,HIGH);

#if defined (CORE_TEENSY)  //only include these lines when compiling for a Teensy board
  usb_midi_write_packed(((m>>4) & 0xF) | (((m | c) & 0xFF) << 8) | ((d1 & 0x7F) << 16));
#elif defined(USBCON)  //only include these lines when compiling for an Arduino if you're compiling for an Arduino that has USB connection in the main MCU but is not a Teensy
  midiEventPacket_t msg = {(m>>4)&0xF, (m | c)&0xFF, d1&0x7F, 0};
  MidiUSB.sendMIDI(msg);
  MidiUSB.flush();
#else // If you're compiling for an Arduino that has no USB connection in the main MCU
  //* The format of the message to send via serial. We create a new data structure, that can store 2 bytes at once.  This will be easier to send as MIDI. */
  typedef struct {
    unsigned int channel : 4;   // second nibble : midi channel (0-15) (channel and status are swapped, because Arduino is Little Endian)
    unsigned int status : 4;    // first  nibble : status message 
    uint8_t data;               // second byte   : data  (0-127)
  } t_midiMsg;          // We call this structure 't_midiMsg'

  t_midiMsg msg;
  msg.status = (m>>4) & 0xF;
  msg.channel = c & 0xF;
  msg.data = d1 & 0x7F;
  Serial.write((uint8_t *)&msg, sizeof(msg));  // Send the MIDI message.
#endif

  if(blinkDelay != 0)
    delay(blinkDelay);
  if (blinkEn)
    digitalWrite(blinkPin,LOW);
}

void USBMidi::setDelay(int d) {
    blinkDelay = d;
}

void USBMidi::blink(byte p) { // pin
  blinkPin = p;
  pinMode(blinkPin, OUTPUT);
  blinkEn = true;
}

void USBMidi::noBlink() {
  if(blinkEn) {
    pinMode(blinkPin, INPUT);
    blinkEn = false;
  }    
}


