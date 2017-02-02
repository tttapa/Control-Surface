#include "Digital.h"
#include "sendMidi.h"
#include "Arduino.h"

//#define NOTE_OFF       0x80
//#define NOTE_ON        0x90
//#define CC             0xB0

Digital::Digital(byte p, byte n, byte c, byte v){ // pin, controller number, channel, velocity
  validPin = true;
#if defined (__AVR_ATmega328P__) // if you're compiling for Arduino Uno
  if(p < 2 || ( p > 13 && p < A0 ) || p > A5){ //If the user enters an invalid pin number. 
    validPin = false;
  }
#endif

#if defined (__AVR_ATmega32U4__) // if you're compiling for Arduino Leonardo
  if( (p < 0 || ( p > 13 && p < A0 ) || p > A11) && p != A10 && p != A11 ){ //If the user enters an invalid pin number. 
    validPin = false;
  }
  if(p == 28) p = 10; // A10 is defined as D28 and D10. However, digital read on D28 does not work.
  if(p == 29) p = 12; // A11 is defined as D29 and D12. However, digital read on D29 does not work.
#endif
  if(validPin) pinMode(p, INPUT_PULLUP);
  pin = p; 
  note = n; 
  channel = c;
  velocity = v;
  oldVal = -1;
  bankTrue = false;
}    

void Digital::refresh(){
  if(validPin){ // if the pin configuration is valid:
    value = digitalRead(pin);
    if(value != oldVal){
      if(value == 0){  // If the button is pressed
        if(bankTrue && !digitalRead(bankPin)){
          sendMidi(NOTE_ON, newChannel, newNote, velocity);
        } 
        else  {
          sendMidi(NOTE_ON, channel, note, velocity);
        }          
      } 
      else {  // If the button is not pressed
        if(bankTrue && !digitalRead(bankPin)){
          sendMidi(NOTE_OFF, newChannel, newNote, velocity);
        } 
        else {
          sendMidi(NOTE_OFF, channel, note, velocity);
        }
      }
      oldVal = value;
    }
  }
  else { // if there is an invalid pin configuration: 
    pinMode(13,OUTPUT);
    for(;;){ // forever 
      digitalWrite(13, 1); // blink led on 13 @1Hz
      delay(500);
      digitalWrite(13, 0);
      delay(500);
    }
  }
}

  void Digital::bank(byte bPin, byte newN, byte newC)  // bank pin, new controller or note number, new channel
  { 
#if defined (__AVR_ATmega328P__) // if you're compiling for Arduino Uno
    if(bPin < 2 || ( bPin > 13 && bPin < 18 ) || bPin > 29){ //If the user enters an invalid pin number. 
      validPin = false;
    }
#endif

#if defined (__AVR_ATmega32U4__) // if you're compiling for Arduino Leonardo
    if( ( bPin < 0 || ( bPin > 13 && bPin < A0 ) || bPin > A9 ) && bPin != A10 && bPin != A11){ //If the user enters an invalid pin number. 
      validPin = false;
    }
    if(bPin == 28) bPin = 10; // A10 is defined as D28 and D10. However, digital read on D28 does not work.
    if(bPin == 29) bPin = 12; // A11 is defined as D29 and D12. However, digital read on D29 does not work.
#endif

    bankTrue = true;

    bankPin = bPin;
    if(validPin) pinMode(bankPin, INPUT_PULLUP); // only enable internal pullup resistor if the pin number is valid.
    newNote = newN; 
    newChannel = newC;
  }

void Digital::detachBank()
{
  bankTrue = false;
  if(validPin) pinMode(bankPin, INPUT); // if the pin was a valid pin, make it a normal input again, without the internal pullup resistor.
}





