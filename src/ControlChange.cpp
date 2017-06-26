#include "Arduino.h"
#include "ControlChange.h"
#include "USBMidi.h"

ControlChange::ControlChange(uint8_t n, uint8_t c) // controller number, channel
{ 
  controller = n; 
  channel = c;
  oldVal = -1;
}

ControlChange::~ControlChange()
{
  free(avValues);
  if(bankEnabled){
    pinMode(bankPin, INPUT); // make it a normal input again, without the internal pullup resistor.
  }
}

void ControlChange::average(size_t len) {
  if(len == 0 || len == 1 || av)
    return;
  avValues = (unsigned int *) malloc(len*sizeof(unsigned int));
  memset(avValues, 0, len*sizeof(unsigned int));
  av = len;
}

void ControlChange::refresh(uint8_t input)
{
  if(av) {
    input = runningAverage(input);
  }  
  value = input & 0x7F;
  if(value != oldVal)
  {
    if(bankEnabled && !digitalRead(bankPin))
    {
      USBMidiController.send(CC, newChannel, newController, value);
    } 
    else {
      USBMidiController.send(CC, channel, controller, value);
    }
    oldVal = value;
  }
}

void ControlChange::refresh(float input)
{
  refresh((uint8_t)round(127.0*input));
}

void ControlChange::bank(uint8_t bPin, uint8_t newN, uint8_t newC)  // (digital) bank pin, new controller number, new channel
{
  bankEnabled = true;
  
  bankPin = bPin;
  pinMode(bankPin, INPUT_PULLUP);
  newController = newN; 
  newChannel = newC;
}

void ControlChange::detachBank()
{
  if(bankEnabled){
    bankEnabled = false;
    pinMode(bankPin, INPUT); // make it a normal input again, without the internal pullup resistor.
  }
}

unsigned int ControlChange::runningAverage(int M) { // http://playground.arduino.cc/Main/RunningAverage
    // keep sum updated to improve speed.
    avSum -= avValues[avIndex];
    avValues[avIndex] = M;
    avSum += avValues[avIndex];
    avIndex++;
    avIndex = avIndex % av;
    if (avCount < av) avCount++;

    return avSum / avCount;
  }