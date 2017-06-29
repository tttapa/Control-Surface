#include "Arduino.h"
#include "AnalogHiRes.h"
#include "USBMidi.h"

AnalogHiRes::AnalogHiRes(byte p, byte c) // pin, channel
{ 
  analogPin = p; 
  channel = c;
  oldVal = -1;
}

AnalogHiRes::~AnalogHiRes()
{
  free(avValues);
  if(bankTrue){
    pinMode(digitalPin, INPUT); // make it a normal input again, without the internal pullup resistor.
  }
}

void AnalogHiRes::average(size_t len) {
  if(len == 0 || len == 1 || av)
    return;
  avValues = (unsigned int *) malloc(len*sizeof(unsigned int));
  memset(avValues, 0, len*sizeof(unsigned int));
  av = len;
}

void AnalogHiRes::refresh()
{
#if defined(CORE_TEENSY) || defined(__arm__) // Teensy boards and Arduino Due support higher resolutions than AVR boards
  analogReadResolution(14);
  uint16_t input = analogRead(analogPin);
  analogReadResolution(10);
#else // Analog resolution is only 10 bits
  uint16_t input = analogRead(analogPin)<<4; // make it a 14-bit number
#endif

  value = AnalogHiResMap(input);

  if(av) {
    value = runningAverage(value);
  }

  if(value != oldVal)
  {
    if(bankTrue && !digitalRead(digitalPin))
    {
      USBMidiController.send(PITCH_BEND, newChannel, value, value>>7);
    } 
    else {
      USBMidiController.send(PITCH_BEND, channel, value, value>>7);
    }
    oldVal = value;
  }
}

void AnalogHiRes::map(uint16_t (*fn)(uint16_t)) {
  AnalogHiResMap = fn;
}

void AnalogHiRes::bank(byte dPin, byte newC)  // digital pin, new channel
{
  bankTrue = true;
  
  digitalPin = dPin;
  pinMode(digitalPin, INPUT_PULLUP);
  newChannel = newC;
}

void AnalogHiRes::detachBank()
{
  if(bankTrue){
    bankTrue = false;
    pinMode(digitalPin, INPUT); // make it a normal input again, without the internal pullup resistor.
  }
}

unsigned int AnalogHiRes::runningAverage(int M) { // http://playground.arduino.cc/Main/RunningAverage
    // keep sum updated to improve speed.
    avSum -= avValues[avIndex];
    avValues[avIndex] = M;
    avSum += avValues[avIndex];
    avIndex++;
    avIndex = avIndex % av;
    if (avCount < av) avCount++;

    return avSum / avCount;
  }