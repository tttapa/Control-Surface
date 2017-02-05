#include "Arduino.h"
#include "Analog.h"
#include "sendMidi.h"

Analog::Analog(byte p, byte n, byte c, byte r) // pin, controller number, channel, resolution
{ 
  analogPin = p; 
  controller = n; 
  channel = c;
  resolution = r;
  oldVal = -1;
  bankTrue = false;
  if(resolution < 2) resolution = 2;
  if(resolution > 128) resolution = 128;

}

void Analog::refresh(MIDISender& s)
{
  value = map(analogRead(analogPin),0,1024, 0, resolution);
  value = map(value, 0, resolution-1, 0, 127);
  if(value != oldVal)
  {
    if(bankTrue && !digitalRead(digitalPin))
    {
      USB.send(CC, newChannel, newController, value);
    } 
    else {
      USB.send(CC, channel, controller, value);
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


