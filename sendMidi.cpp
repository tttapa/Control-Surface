#include "Arduino.h"
#include "sendMidi.h"

int Delay;
byte pin;
boolean Blink;

void setupMidi(byte p, int d){
#ifdef DEBUG
  Serial.begin(9600); //Start serial communication for debugging purposes @9600 baud
#else
//#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__) //only include these lines when compiling for Arduino Uno or Arduino Mega (that have an ATmega16U2 for serial communication)
#if !defined(USBCON)
  Serial.begin(31250); // Start communication with ATmega16U2 @31250 baud (for MIDI firmware: )
#endif
#endif

  Blink = true;  
#if !defined(USBCON) // if you're compiling for an Arduino that has no USB connection in the main MCU
  if(p == 0 || p == 1){ //If the user enters an invalid pin number (pins 0 and 1 are used for Serial communication) 
    Blink = false;
  }
#endif
  
  pin = p;
  if(Blink) pinMode(p,OUTPUT);  // The pin with the LED connected is set to output.
  
  Delay = d;
}

void sendMidi(byte m, byte c, byte d1, byte d2) // Custom function to send MIDI messages: message, channel, data1, data2
{
  if(Blink) digitalWrite(pin,1);
  
#ifdef DEBUG
    Serial.print(m, HEX); 
    Serial.print('\t');
    Serial.print(c, HEX);   
    Serial.print('\t');
    Serial.print(d1, HEX); 
    Serial.print('\t');
    Serial.print(d2, HEX); 
    Serial.print('\n');
#else

#if defined(USBCON)  //only include these lines when compiling for Arduino Leonardo
    if(m == NOTE_ON){
      usbMIDI.sendNoteOn(d1, d2, c);
    } 
    else if(m == NOTE_OFF){
      usbMIDI.sendNoteOff(d1, d2, c);
    } 
    else if(m == CC){
      usbMIDI.sendControlChange(d1, d2, c);
    }
#endif

#if defined (CORE_TEENSY)  //only include these lines when compiling for a Teensy board
    if(m == NOTE_ON){
      usbMIDI.sendNoteOn(d1, d2, c);
    } 
    else if(m == NOTE_OFF){
      usbMIDI.sendNoteOff(d1, d2, c);
    } 
    else if(m == CC){
      usbMIDI.sendControlChange(d1, d2, c);
    }
#endif

#if defined (__AVR_ATmega328P__)  //only include these lines when compiling for Arduino Uno
    /* The format of the message to send via serial. We create a new data type, that can store 4 values at once.  This will be easier to send as MIDI. */
    typedef struct {
      uint8_t command;  // first 4 bits  : channel message (NOTE_ON, NOTE_OFF or CC (controlchange)
      uint8_t channel;  // second 4 bits : midi channel (0-15)
      uint8_t data1;    // second byte   : first value (0-127), controller number or note number
      uint8_t data2;    // third byte    : second value (0-127), controller value or velocity
    } 
    t_midiMsg;          // We call this data type 't_midiMsg'

      t_midiMsg msg;
    msg.command = m;
    msg.channel = c;
    msg.data1   = d1;
    msg.data2   = d2;
    Serial.write((uint8_t *)&msg, sizeof(msg));  // Send the MIDI message.
#endif

#endif
  if(Delay != 0){
    delay(abs(Delay)); // Prevent an overflow of MIDI messages. Increase to increase stability, decrease to achieve faster response.
  }
  if(Blink) digitalWrite(pin,0);
}


void sendMidi(byte m, byte c, int d1) // Custom function to send MIDI messages: message, channel, data1
{
  if(Blink) digitalWrite(pin,1);
  
#ifdef DEBUG
    Serial.print(m, HEX); 
    Serial.print('\t');
    Serial.print(c, HEX);   
    Serial.print('\t');
    Serial.print(d1, HEX); 
    Serial.print('\n');
#else

#if defined (__AVR_ATmega32U4__)  //only include these lines when compiling for Arduino Leonardo
    if(m == PROGRAM_CHANGE){
      usbMIDI.sendProgramChange(d1, c);
    }
    else if(m == PITCH_BEND){
      usbMIDI.sendPitchBend(d1+8192, c); //0=-8192; 8192=0; 16383=8191
    }
#endif

#if defined (__AVR_ATmega328P__)  //only include these lines when compiling for Arduino Uno
    /* The format of the message to send via serial. We create a new data type, that can store 4 values at once.  This will be easier to send as MIDI. */
    if(m == PROGRAM_CHANGE){
      typedef struct {
        uint8_t command;  // first 4 bits  : channel message (NOTE_ON, NOTE_OFF or CC (controlchange)
        uint8_t channel;  // second 4 bits : midi channel (0-15)
        uint8_t data1;    // second byte   : first value (0-127), controller number or note number
      } 
      t_midiMsg;          // We call this data type 't_midiMsg'

        t_midiMsg msg;
      msg.command = m;
      msg.channel = c;
      msg.data1   = d1;
      Serial.write((uint8_t *)&msg, sizeof(msg));  // Send the MIDI message.
    } else if(m == PITCH_BEND){
      typedef struct {
        uint8_t command;  // first 4 bits  : channel message (NOTE_ON, NOTE_OFF or CC (controlchange)
        uint8_t channel;  // second 4 bits : midi channel (0-15)
        uint8_t data1;    // second byte   : first value (0-127),  7 least significant bits
        uint8_t data2;    // third byte    : second value (0-127), 7 most  significant bits   
      } 
      t_midiMsg;          // We call this data type 't_midiMsg'

        t_midiMsg msg;
      msg.command = m;
      msg.channel = c;
      msg.data2   = (d1+8192)/128; // seven most significant bits
      msg.data1   = d1+8192-(msg.data2*128);  // seven least significant bits
      Serial.write((uint8_t *)&msg, sizeof(msg));  // Send the MIDI message.
    }
#endif

#endif
  if(Delay != 0){
    delay(abs(Delay)); // Prevent an overflow of MIDI messages. Increase to increase stability, decrease to achieve faster response.
  }
  if(Blink) digitalWrite(pin,0);
}

