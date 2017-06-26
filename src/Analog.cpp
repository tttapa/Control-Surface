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

Analog::~Analog()
{
  free(avValues);
  if(bankTrue){
    pinMode(digitalPin, INPUT); // make it a normal input again, without the internal pullup resistor.
  }
}

void Analog::average(size_t len) {
  if(len == 0 || len == 1 || av)
    return;
  avValues = (unsigned int *) malloc(len*sizeof(unsigned int));
  memset(avValues, 0, len*sizeof(unsigned int));
  av = len;
}

void Analog::refresh()
{
  unsigned int input = analogMap(analogRead(analogPin));
  if(av) {
    input = runningAverage(input);
  }  
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

void Analog::map(TmapFunction fn) {
  analogMap = fn;
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
  if(bankTrue){
    bankTrue = false;
    pinMode(digitalPin, INPUT); // make it a normal input again, without the internal pullup resistor.
  }
}

unsigned int Analog::runningAverage(int M) { // http://playground.arduino.cc/Main/RunningAverage
    // keep sum updated to improve speed.
    avSum -= avValues[avIndex];
    avValues[avIndex] = M;
    avSum += avValues[avIndex];
    avIndex++;
    avIndex = avIndex % av;
    if (avCount < av) avCount++;

    return avSum / avCount;
  }