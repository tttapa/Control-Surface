#include "Arduino.h"
#include "Analog.h"
#include "sendMidi.h"

//#define CC 0xB0

Analog::Analog(byte p, byte n, byte c, byte r) // pin, controller number, channel, resolution
{ 
  validPin = true;
#if defined (__AVR_ATmega328P__) // if you're compiling for Arduino Uno
  if(p < A0 || p > A5){ //If the user enters an invalid pin number. 
    validPin = false;
  }
#endif

#if defined (__AVR_ATmega32U4__) // if you're compiling for Arduino Leonardo
  if((p < A0 || p > A9) && p != A10 && p != A11){ //If the user enters an invalid pin number. You'd expect p < A0 || p > A11, but this does not work, due to the way the analog input constants are implemented. (A10 = D10 and A10 = D28)
    validPin = false;    
  }
#endif

  analogPin = p; 
  controller = n; 
  channel = c;
  resolution = r;
  oldVal = -1;
  bankTrue = false;
  if(resolution < 2) resolution = 2;
  if(resolution > 128) resolution = 128;

}

void Analog::refresh()
{
  if(validPin){ // if the pin configuration is valid:
    value = map(analogRead(analogPin),0,1024, 0, resolution);
    value = map(value, 0, resolution-1, 0, 127);
    if(value != oldVal)
    {
      if(bankTrue && !digitalRead(digitalPin))
      {
        sendMidi(CC, newChannel, newController, value);
      } 
      else {
        sendMidi(CC, channel, controller, value);
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

void Analog::bank(byte dPin, byte newN, byte newC)  // digital pin, new controller number, new channel
{ 
#if defined (__AVR_ATmega328P__) // if you're compiling for Arduino Uno
  if(dPin < 2 || ( dPin > 13 && dPin < 18 ) || dPin > 29){ //If the user enters an invalid pin number. 
    validPin = false;
  }
#endif

#if defined (__AVR_ATmega32U4__) // if you're compiling for Arduino Leonardo
  if( ( dPin < 0 || ( dPin > 13 && dPin < A0 ) || dPin > A9 ) && dPin != A10 && dPin != A11){ //If the user enters an invalid pin number. 
    validPin = false;
  }
  if(dPin == 28) dPin = 10; // A10 is defined as D28 and D10. However, digital read on D28 does not work.
  if(dPin == 29) dPin = 12; // A11 is defined as D29 and D12. However, digital read on D29 does not work.
#endif

  bankTrue = true;
  
  digitalPin = dPin;
  if(validPin) pinMode(digitalPin, INPUT_PULLUP); // only enable internal pullup resistor if the pin number is valid.
  newController = newN; 
  newChannel = newC;
}

void Analog::detachBank()
{
  bankTrue = false;
  if(validPin) pinMode(digitalPin, INPUT); // if the pin was a valid pin, make it a normal input again, without the internal pullup resistor.
}


