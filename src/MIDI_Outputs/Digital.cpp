#include "Digital.h"
#include "../Control_Surface/Control_Surface_Class.h"

using namespace ExtIO;

Digital::Digital(pin_t pin, uint8_t note, uint8_t channel, uint8_t velocity) // Constructor
{
  pinMode(pin, INPUT_PULLUP); // Enable the internal pull-up resistor on the pin with the button/switch
  this->pin = pin;
  this->note = note;
  this->channel = channel;
  this->velocity = velocity;
}

Digital::~Digital() // Destructor
{
  pinMode(pin, INPUT); // make it a normal input again, without the internal pullup resistor.
}

void Digital::invert() // Invert the button state (send Note On event when released, Note Off when pressed)
{
  invertState = true;
}

void Digital::refresh() // Check if the button state changed, and send a MIDI Note On or Off accordingly
{
  bool state = digitalRead(pin) ^ invertState; // read the button state and invert it if "invert" is true

  if (millis() - prevBounceTime > debounceTime)
  {
    int8_t stateChange = state - buttonState;

    if (stateChange == falling)
    { // Button is pushed
      buttonState = state;
      Control_Surface.MIDI()->send(NOTE_ON, channel + channelOffset * tracksPerBank, note + addressOffset * tracksPerBank, velocity);
    }

    if (stateChange == rising)
    { // Button is released
      buttonState = state;
      Control_Surface.MIDI()->send(NOTE_OFF, channel + channelOffset * tracksPerBank, note + addressOffset * tracksPerBank, velocity);
      if (newChannelOffset != channelOffset || newAddressOffset != addressOffset)
      {
        channelOffset = newChannelOffset;
        addressOffset = newAddressOffset;
      }
    }
  }
  if (state != prevState)
  {
    prevBounceTime = millis();
    prevState = state;
  }
}

void Digital::setChannelOffset(uint8_t offset) // Set the channel offset
{
  if (buttonState == HIGH) // Button isn't pressed
    channelOffset = offset;
  newChannelOffset = offset;
}

void Digital::setAddressOffset(uint8_t offset) // Set the address (note or controller number) offset
{
  if (buttonState == HIGH) // Button isn't pressed
    addressOffset = offset;
  newAddressOffset = offset;
}