#include "Arduino.h"
#include "Analog.h"
#include "MIDI_controller.h"

Analog::Analog(uint8_t analogPin, uint8_t controllerNumber, uint8_t channel) // Constructor
{
  this->analogPin = analogPin;
  this->controllerNumber = controllerNumber;
  this->channel = channel;
}

Analog::~Analog() // Destructor
{
  free(avValues); // free the sample buffer malloc'ed in Analog::average
}
#ifdef SINGLE_BYTE_AVERAGE
void Analog::average(uint8_t length) // use the average of multiple samples of analog readings
{
  if (length == 0 || length == 1 || avLen) // the average of 0 or 1 samples is meaningless, if "av" already exists, don't allocate new memory
    return;
  avValues = (uint8_t *)malloc(length * sizeof(uint8_t)); // allocate memory for the sample buffer
  memset(avValues, 0, length * sizeof(uint8_t));          // set all values in the buffer to zero
  avLen = length;
}
#else
void Analog::average(size_t length) // use the average of multiple samples of analog readings
{
  if (length == 0 || length == 1 || avLen) // the average of 0 or 1 samples is meaningless, if "av" already exists, don't allocate new memory
    return;
  avValues = (unsigned int *)malloc(length * sizeof(unsigned int)); // allocate memory for the sample buffer
  memset(avValues, 0, length * sizeof(unsigned int));               // set all values in the buffer to zero
  avLen = length;
}
#endif

void Analog::refresh() // read the analog value, update the average, map it to a MIDI value, check if it changed since last time, if so, send Control Change message over MIDI
{
  unsigned int input = analogRead(analogPin); // read the raw analog input value
  input = analogMap(input);                   // apply the analogMap function to the value (identity function f(x) = x by default)

#ifdef SINGLE_BYTE_AVERAGE    // use 8-bit value for averaging
  uint8_t value = input >> 2; // map from the 10-bit analog input value [0, 1023] to the 8-bit value [0, 255]
  if (avLen)
  {                                // if averaging is enabled
    value = runningAverage(value); // update the running average with the new value
  }
  value = value >> 1; // map from 8-bit value [0, 255] to the 7-bit MIDI value [0, 127]
#else // use raw 10-bit input value for averaging (int)
  if (avLen)
  {                                // if averaging is enabled
    input = runningAverage(input); // update the running average with the new value
  }
  uint8_t value = input >> 3; // map from the 10-bit analog input value [0, 1023] to the 7-bit MIDI value [0, 127]
#endif
  if (value != oldVal) // if the value changed since last time
  {
    MIDI_Controller.MIDI()->send(CC, channel + channelOffset * channelsPerBank, controllerNumber + addressOffset * channelsPerBank, value); // send a Control Change MIDI event
    oldVal = value;
  }
}

void Analog::map(int (*fn)(int)) // change the function pointer for analogMap to a new function. It will be applied to the raw analog input value in Analog::refresh()
{
  analogMap = fn;
}
#ifdef SINGLE_BYTE_AVERAGE
uint8_t Analog::runningAverage(uint8_t value) // http://playground.arduino.cc/Main/RunningAverage
#else
unsigned int Analog::runningAverage(unsigned int value) // http://playground.arduino.cc/Main/RunningAverage
#endif
{
  avSum -= avValues[avIndex];
  avValues[avIndex] = value;
  avSum += value;
  avIndex++;
  avIndex = avIndex % avLen;
  if (avCount < avLen)
    avCount++;
  return avSum / avCount;
}