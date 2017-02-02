#include "Arduino.h"
#include "sendMidi.h"

#if defined (USBCON)
#include "MIDIUSB.h"
#endif

int Delay;
byte pin;
bool Blink;
bool Debug;

void setupMidi(byte p, int d, bool db){
//#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__) // Only include these lines when compiling for Arduino Uno or Arduino Mega (that have an ATmega16U2 for serial communication)
#if !defined(USBCON) // If you're compiling for an Arduino that has USB connection in the main MCU
  if(db)
    Serial.begin(9600); // a baud rate supported by the computer, for debugging
  else
    Serial.begin(31250); // Start communication with ATmega16U2 @31250 baud (for MIDI firmware: )
#else 
  if(db)
    Serial.begin(9600); // a baud rate supported by the computer, for debugging
#endif

  Blink = true;  
#if !defined(USBCON) // If you're compiling for an Arduino that has no USB connection in the main MCU
  if(p == 0 || p == 1){ // If the user enters an invalid pin number (pins 0 and 1 are used for Serial communication) 
    Blink = false;
  }
#endif
  
  pin = p;
  if(Blink) pinMode(p,OUTPUT);  // The pin with the LED connected is set to output.
  
  Delay = d;
  Debug = db;
}

void sendMidi(byte m, byte c, byte d1, byte d2) // Custom function to send MIDI messages: message, channel, data1, data2
{
  if(Blink) digitalWrite(pin,1);
  
  if(Debug){
    Serial.print((m>>4)&0xF, HEX); 
    Serial.print('\t');
    Serial.print(c, HEX);   
    Serial.print('\t');
    Serial.print(d1, HEX); 
    Serial.print('\t');
    Serial.print(d2, HEX); 
    Serial.print('\n');
  } else {
    
#if defined (CORE_TEENSY)  //only include these lines when compiling for a Teensy board
    usb_midi_write_packed(((m>>4) & 0xF) | (((m | c) & 0xFF) << 8)
		  | ((d1 & 0x7F) << 16) | ((d2 & 0x7F) << 24));
#elif defined(USBCON)  //only include these lines when compiling for an Arduino if you're compiling for an Arduino that has no USB connection in the main MCU and is not a Teensy
    midiEventPacket_t msg = {(m>>4)&0xF, (m | c)&0xFF, d1&0x7F, d2&0x7F};
    MIDIUSB.sendMIDI(msg);
#else // If you're compiling for an Arduino that has USB connection in the main MCU

    //* The format of the message to send via serial. We create a new data structure, that can store 3 bytes at once.  This will be easier to send as MIDI. */
    typedef struct {
      unsigned int channel : 4;   // second nibble : midi channel (0-15) (channel and status are swapped, because Arduino is Little Endian)
      unsigned int status : 4;    // first  nibble : status message (NOTE_ON, NOTE_OFF or CC (controlchange) 
      uint8_t data1;              // second byte   : first value (0-127), controller number or note number
      uint8_t data2;              // third  byte   : second value (0-127), controller value or velocity
    } 
    t_midiMsg;          // We call this structure 't_midiMsg'

    t_midiMsg msg;
    msg.status = m & 0xF;
    msg.channel = c & 0xF; // channels are 0-based
    msg.data1 = d1 & 0x7F;
    msg.data2 = d2 & 0x7F;
    Serial.write((uint8_t *)&msg, sizeof(msg));  // Send the MIDI message.
#endif
  }

  if(Delay != 0){
    delay(Delay); // Prevent an overflow of MIDI messages. Increase to increase stability, decrease to achieve faster response.
  }
  if(Blink) digitalWrite(pin,0);
}


void sendMidi(byte m, byte c, int d1) // Custom function to send MIDI messages: message, channel, data1
{
 if(Blink) digitalWrite(pin,1);
  
  if(Debug){
    Serial.print((m>>4)&0xF, HEX); 
    Serial.print('\t');
    Serial.print(c, HEX);   
    Serial.print('\t');
    Serial.print(d1, HEX); 
    Serial.print('\n');
  } else {
    
#if defined (CORE_TEENSY)  //only include these lines when compiling for a Teensy board
    usb_midi_write_packed(((m>>4) & 0xF) | (((m | c) & 0xFF) << 8)
		  | ((d1 & 0x7F) << 16));
#elif defined(USBCON)  //only include these lines when compiling for an Arduino if you're compiling for an Arduino that has no USB connection in the main MCU and is not a Teensy
    midiEventPacket_t msg = {(m>>4)&0xF, (m | c)&0xFF, d1&0x7F, 0};
    MIDIUSB.sendMIDI(msg);
#else // If you're compiling for an Arduino that has USB connection in the main MCU

    //* The format of the message to send via serial. We create a new data structure, that can store 2 bytes at once.  This will be easier to send as MIDI. */
    typedef struct {
      unsigned int channel : 4;   // second nibble : midi channel (0-15) (channel and status are swapped, because Arduino is Little Endian)
      unsigned int status : 4;    // first  nibble : status message 
      uint8_t data;              // second byte   : data  (0-127)
    } 
    t_midiMsg;          // We call this structure 't_midiMsg'

    t_midiMsg msg;
    msg.status = m & 0xF;
    msg.channel = c & 0xF; // channels are 0-based
    msg.data = d1 & 0x7F;
    Serial.write((uint8_t *)&msg, sizeof(msg));  // Send the MIDI message.
#endif
  }

  if(Delay != 0){
    delay(Delay); // Prevent an overflow of MIDI messages. Increase to increase stability, decrease to achieve faster response.
  }
  if(Blink) digitalWrite(pin,0);
}

