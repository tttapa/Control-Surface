#include "Arduino.h"
#include "Analog.h"
#include "USBMidi.h"

Analog::Analog(byte p, byte n, byte c) // pin, controller number, channel
{ 
  analogPin = p; 
  controller = n; 
  channel = c;
  oldVal = -1;
}
Analog::Analog(byte p, byte n, byte c, byte r) // pin, controller number, channel, (resolution) for compatibility only, will be removed in next revision
{
  Analog(p,n,c);
}

void Analog::refresh()
{
  int input = analogRead(analogPin);
#ifdef ANALOG_AVERAGE
  input = runningAverage(input);
  Serial.println(input);
  Serial.flush();
#endif // ANALOG_AVERAGE
  value = input >> 3;
  if(value != oldVal)
  {
    if(bankTrue && !digitalRead(digitalPin))
    {
      USBMidiController.send(CC, newChannel, newController, value);
    } 
    else {
      USBMidiController.send(CC, channel, controller, value);
    }
    oldVal = value;
  }
}

void Analog::bank(byte dPin, byte newN, byte newC)  // digital pin, new controller number, new channel
{
  bankTrue = true;
  
  digitalPin = dPin;
  pinMode(digitalPin, INPUT_PULLUP);
  newController = newN; 
  newChannel = newC;
}

void Analog::detachBank()
{
  bankTrue = false;
  pinMode(digitalPin, INPUT); // make it a normal input again, without the internal pullup resistor.
}