#include "DigitalLatch.h"

DigitalLatch::DigitalLatch(uint8_t pin, uint8_t note, uint8_t channel, uint8_t velocity, unsigned long latchTime) // Constructor
{
  pinMode(pin, INPUT_PULLUP); // Enable the internal pull-up resistor on the pin with the button/switch
  this->pin = pin;
  this->note = note;
  this->channel = channel;
  this->velocity = velocity;
  this->latchTime = latchTime;
}

DigitalLatch::~DigitalLatch() // Deconstructor
{
  pinMode(pin, INPUT); // make it a normal input again, without the internal pullup resistor.
}

void DigitalLatch::refresh() // Check if the button state changed, if so, send a MIDI Note On, after a non-blocking delay of "latchTime", send a Note Off
{
  bool state = digitalRead(pin); // read the button state
  if (state != oldState)         // If the switch changed position
  {
    if (noteOffSent) // If the note is turned off
    {
      USBMidiController.send(NOTE_ON, channel + channelOffset, note + addressOffset, velocity); // Turn on the note
      noteOnTime = millis();                                                                    // store the time of the note on message
      noteOffSent = false;                                                                      // The note is turned on
    }
    else // If the button is switched again, before latch time is reached
    {
      USBMidiController.send(NOTE_OFF, channel + channelOffset, note + addressOffset, velocity); // Turn off the note
      USBMidiController.send(NOTE_ON, channel + channelOffset, note + addressOffset, velocity);  // Immediately turn the note on again
      noteOnTime = millis();                                                                     // store the time of the note on message
    }
    oldState = state;
  }
  if (millis() - noteOnTime > latchTime && !noteOffSent) // if the time elapsed since the Note On event is greater than the latch time, and if the note is still on
  {
    USBMidiController.send(NOTE_OFF, channel + channelOffset, note + addressOffset, velocity); // Turn off the note
    noteOffSent = true;                                                                        // The note is turned off
  }
}