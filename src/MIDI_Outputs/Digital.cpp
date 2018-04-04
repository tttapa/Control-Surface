#include "Digital.h"
#include "../Control_Surface/Control_Surface_Class.h"

using namespace ExtIO;

Digital::Digital(pin_t pin, uint8_t address, uint8_t channel, uint8_t velocity, MessageType messageType) // Constructor
  : pin(pin), address(address), channel(channel), velocity(velocity), messageType(messageType)
{
  pinMode(pin, INPUT_PULLUP); // Enable the internal pull-up resistor on the pin with the button/switch
}

Digital::~Digital() // Destructor
{
  pinMode(pin, INPUT); // make it a normal input again, without the internal pullup resistor.
}

void Digital::invert() // Invert the button state (send Note On event when released, Note Off when pressed)
{
  invertState = true;
}
bool Digital::invertState = false;

void Digital::refresh() // Check if the button state changed, and send a MIDI Note On or Off accordingly
{
  bool state = digitalRead(pin) ^ invertState; // read the button state and invert it if "invertState" is true

  if (millis() - prevBounceTime > debounceTime)
  {
    int8_t stateChange = state - buttonState;

    if (stateChange == falling) // Button is pushed
    {
      buttonState = state;
      Control_Surface.MIDI()->send(messageType == NOTE ? NOTE_ON : CONTROL_CHANGE, 
                                   channel + channelOffset * tracksPerBank, 
                                   address + addressOffset * tracksPerBank, 
                                   velocity);
    }

    if (stateChange == rising) // Button is released
    {
      buttonState = state;
      Control_Surface.MIDI()->send(messageType == NOTE ? NOTE_OFF : CONTROL_CHANGE,
                                   channel + channelOffset * tracksPerBank, 
                                   address + addressOffset * tracksPerBank, 
                                   messageType == NOTE ? velocity : 0);
      if (newChannelOffset != channelOffset || newAddressOffset != addressOffset)
      {
        channelOffset = newChannelOffset;
        addressOffset = newAddressOffset;
      }
    }
  }
  if (state != prevState) // Button is pushed, released or bounces
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