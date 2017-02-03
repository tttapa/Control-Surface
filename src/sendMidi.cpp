#include "Arduino.h"
#include "sendMidi.h"

#if defined (USBCON)
#include "MIDIUSB.h"
#endif

#define MIDI_BAUD 31250

MIDISender::MIDISender(byte p, int d, Stream s) {
#if ! (defined(USBCON) || defined (CORE_TEENSY))
  midiStream = s == null ? Serial : s;
#else 
  midiStream = s;
#endif
//#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__) // Only include these lines when compiling for Arduino Uno or Arduino Mega (that have an ATmega16U2 for serial communication)
#if ! (defined(USBCON) || defined (CORE_TEENSY)) // If you're compiling for an Arduino that has USB connection in the main MCU
    midiStream.begin(MIDI_BAUD); // Start communication with ATmega16U2 @31250 baud (for MIDI firmware: Hiduino: https://github.com/ddiakopoulos/hiduino)
#endif 

  if(p != NO_BLINK)
    blink = true;  
#if !defined(USBCON) // If you're compiling for an Arduino that has no USB connection in the main MCU
  if(p == 0 || p == 1){ // If the user enters an invalid pin number (pins 0 and 1 are used for Serial communication) 
    blink = false;
  }
#endif
  
  pin = p;
  if(blink) 
    pinMode(p,OUTPUT);  // The pin with the LED connected is set to output.
  
  delay = d;
}



void sendMidi(byte m, byte c, byte d1, byte d2) // Custom function to send MIDI messages: message, channel, data1, data2
{
  c--; // Channels are zero-based

  if(Blink)
    digitalWrite(pin,1);
    
#if defined (CORE_TEENSY)  //only include these lines when compiling for a Teensy board
  if(midiStream == null){
    usb_midi_write_packed(((m>>4) & 0xF) | (((m | c) & 0xFF) << 8) | ((d1 & 0x7F) << 16) | ((d2 & 0x7F) << 24));
  } else {
    serialMidi(m, c, d1, d2, midiStream);
  }  
#elif defined(USBCON)  //only include these lines when compiling for an Arduino if you're compiling for an Arduino that has USB connection in the main MCU but is not a Teensy
  if(midiStream == null){
    midiEventPacket_t msg = {(m>>4)&0xF, (m | c)&0xFF, d1&0x7F, d2&0x7F};
    MidiUSB.sendMIDI(msg);
    MidiUSB.flush();
  } else {
    serialMidi(m, c, d1, d2, midiStream);
  }
#else // If you're compiling for an Arduino that has no USB connection in the main MCU
  serialMidi(m, c, d1, d2, midiStream);
#endif

  if(Delay != 0){
    delay(Delay); // Prevent an overflow of MIDI messages. Increase to increase stability, decrease to achieve faster response.
  }
  if(Blink)
    digitalWrite(pin,0);
}



void sendMidi(byte m, byte c, int d1) // Custom function to send MIDI messages: message, channel, data1
{
  c--; // Channels are zero-based

  if(Blink)
    digitalWrite(pin,1);
    
#if defined (CORE_TEENSY)  //only include these lines when compiling for a Teensy board
  if(midiStream == null){
    usb_midi_write_packed(((m>>4) & 0xF) | (((m | c) & 0xFF) << 8) | ((d1 & 0x7F) << 16));
  } else {
    serialMidi(m, c, d1, midiStream);
  }  
#elif defined(USBCON)  //only include these lines when compiling for an Arduino if you're compiling for an Arduino that has USB connection in the main MCU but is not a Teensy
  if(midiStream == null){
    midiEventPacket_t msg = {(m>>4)&0xF, (m | c)&0xFF, d1&0x7F, 0};
    MidiUSB.sendMIDI(msg);
    MidiUSB.flush();
  } else {
    serialMidi(m, c, d1, midiStream);
  }
#else // If you're compiling for an Arduino that has no USB connection in the main MCU
  serialMidi(m, c, d1, midiStream);
#endif

  if(Delay != 0){
    delay(Delay); // Prevent an overflow of MIDI messages. Increase to increase stability, decrease to achieve faster response.
  }
  if(Blink)
    digitalWrite(pin,0);
}

MIDISender::serialMidi(byte m, byte c, byte d1, byte d2, Stream s) {
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
  s.write((uint8_t *)&msg, sizeof(msg));  // Send the MIDI message.
}

MIDISender::serialMidi(byte m, byte c, byte d1, Stream s) {
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
  s.write((uint8_t *)&msg, sizeof(msg));  // Send the MIDI message.
}


MIDIDebug::MIDIDebug(byte p, int d, Stream ds, unsigned long baud) {
  debugStream = ds;
  debugStream.begin(baud);
  if(p != NO_BLINK)
    blink = true;  
#if !defined(USBCON) // If you're compiling for an Arduino that has no USB connection in the main MCU
  if(p == 0 || p == 1){ // If the user enters an invalid pin number (pins 0 and 1 are used for Serial communication) 
    blink = false;
  }
#endif
  
  pin = p;
  if(blink) 
    pinMode(p,OUTPUT);  // The pin with the LED connected is set to output.
  
  delay = d;
}

void MIDIDebug::sendMidi(byte m, byte c, byte d1, byte d2) {
  c--; // Channels are zero-based

  if(Blink)
    digitalWrite(pin,1);

  debugStream.print((m>>4)&0xF, HEX); 
  debugStream.print('\t');
  debugStream.print(c, HEX);   
  debugStream.print('\t');
  debugStream.print(d1, HEX); 
  debugStream.print('\t');
  debugStream.print(d2, HEX); 
  debugStream.print('\n');

  if(Delay != 0){
    delay(Delay); // Prevent an overflow of MIDI messages. Increase to increase stability, decrease to achieve faster response.
  }
  if(Blink)
    digitalWrite(pin,0);
}

void MIDIDebug::sendMidi(byte m, byte c, byte d1) {
  c--; // Channels are zero-based

  if(Blink)
    digitalWrite(pin,1);

  debugStream.print((m>>4)&0xF, HEX); 
  debugStream.print('\t');
  debugStream.print(c, HEX);   
  debugStream.print('\t');
  debugStream.print(d1, HEX); 
  debugStream.print('\n');
  
  if(Delay != 0){
    delay(Delay); // Prevent an overflow of MIDI messages. Increase to increase stability, decrease to achieve faster response.
  }
  if(Blink)
    digitalWrite(pin,0);
}
