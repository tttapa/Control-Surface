#include "AnalogHiRes.h"
#include "Control_Surface.h"

using namespace ExtIO;

AnalogHiRes::AnalogHiRes(pin_t analogPin, uint8_t channel) // Constructor
{
  this->analogPin = analogPin;
  this->channel = channel;
}

void AnalogHiRes::refresh() // read the analog value, update the average, map it to a MIDI value, check if it changed since last time, if so, send Control Change message over MIDI
{
  analogRead(analogPin); // throw away first analog reading
  analog_t input = analogRead(analogPin);
  uint16_t value = analogMap(input) << 4; // apply the analogMap function to the value (identity function f(x) = x by default) and make it a 14-bit number (pad with 4 zeros)

  value = runningAverage(value); // update the running average with the new value

  if (value != oldVal) // if the value changed since last time
  {
    Control_Surface.MIDI()->send(PITCH_BEND, channel + channelOffset * channelsPerBank, value, value >> 7); // send a Pitch Bend MIDI event
    oldVal = value;
  }
}

void AnalogHiRes::map(int (*fn)(int)) // change the function pointer for analogMap to a new function. It will be applied to the raw analog input value in Analog::refresh()
{
  analogMap = fn;
}

unsigned int AnalogHiRes::runningAverage(unsigned int value) // http://playground.arduino.cc/Main/RunningAverage
{
  avSum -= avValues[avIndex];
  avValues[avIndex] = value;
  avSum += avValues[avIndex];
  avIndex++;
  avIndex = avIndex % ANALOG_HI_RES_AVERAGE;
  if (avCount < ANALOG_HI_RES_AVERAGE)
    avCount++;

  return avSum / avCount;
}