#ifndef MIDIctrl_h_
#define MIDIctrl_h_

#include "Analog.h"
#include "AnalogHiRes.h"
#include "ControlChange.h"
#include "Digital.h"
#include "DigitalLatch.h"
#include "RotaryEncoder.h"

#include "Bank.h"
#include "BankSelector.h"

#include "MIDI_Interface.h"
#include "USBMIDI_Interface.h"
#include "SerialMIDI_Interface.h"
#include "DebugMIDI_Interface.h"

#include "MIDI_Input_LED.h"

#include "ExtendedInputOutput/ShiftRegisterOut.h"
#include "ExtendedInputOutput/AnalogMultiplex.h"

class MIDI_Controller
{
public:
  static MIDI_Controller &getInstance()
  {
    static MIDI_Controller instance;
    return instance;
  }
  MIDI_Controller(MIDI_Controller const &) = delete;
  void operator=(MIDI_Controller const &) = delete;

  void refresh()
  {
    refreshControls();
    refreshBankSelectors();
    while (refreshMIDI())
      ;
    updateMidiInput();
    refreshInputs();
  }

private:
  MIDI_Controller() {}
  void refreshControls()
  {
    MIDI_Control_Element *element = MIDI_Control_Element::getFirst();
    while (element != nullptr)
    {
      element->refresh();
      element = element->getNext();
    }
  }
  void updateMidiInput()
  {
    if (availableMIDI() == 0)
      return;
    do
    {
      uint8_t header = MIDI_Interface::getDefault()->getNextHeader();
      if (!header)
        return;
      uint8_t messageType = header & 0xF0;
      uint8_t targetChannel = header & 0x0F;
      uint8_t data1 = MIDI_Interface::getDefault()->read();

      if (messageType == CC && data1 == 0x79) // Reset All Controllers
      {
        Serial.println("Reset All Controllers");
        for (MIDI_Input_Element_CC *element = MIDI_Input_Element_CC::getFirst(); element != nullptr; element = element->getNext())
          element->reset();
        for (MIDI_Input_Element_ChannelPressure *element = MIDI_Input_Element_ChannelPressure::getFirst(); element != nullptr; element = element->getNext())
          element->reset();
      }
      else if (messageType == CC && data1 == 0x7B) // All Notes off
      {
        Serial.println("All Notes Off");
        for (MIDI_Input_Element_Note *element = MIDI_Input_Element_Note::getFirst(); element != nullptr; element = element->getNext())
          element->reset();
      }
      else
      {

        Serial.print("New midi message:\t");
        Serial.printf("%02X %02X\r\n", header, data1);

        if (messageType == CC)
        {
          for (MIDI_Input_Element_CC *element = MIDI_Input_Element_CC::getFirst(); element != nullptr; element = element->getNext())
          {
            if (element->update(data1, targetChannel))
            {
              Serial.println("\tMatch");
              break;
            }
          }
        }
        else if (messageType == NOTE_OFF || messageType == NOTE_ON)
        {
          for (MIDI_Input_Element_Note *element = MIDI_Input_Element_Note::getFirst(); element != nullptr; element = element->getNext())
          {
            if (element->update(messageType, data1, targetChannel))
            {
              Serial.println("\tMatch");
              break;
            }
          }
        }
        else if (messageType == CHANNEL_PRESSURE)
        {
          for (MIDI_Input_Element_ChannelPressure *element = MIDI_Input_Element_ChannelPressure::getFirst(); element != nullptr; element = element->getNext())
          {
            if (element->update(targetChannel, data1))
            {
              Serial.println("\tMatch");
              break;
            }
          }
        }
      }
    } while (availableMIDI() > 0);
  }
  void refreshInputs()
  {
    {
      MIDI_Input_Element_CC *element = MIDI_Input_Element_CC::getFirst();
      while (element != nullptr)
      {
        element->refresh();
        element = element->getNext();
      }
    }
    {
      MIDI_Input_Element_Note *element = MIDI_Input_Element_Note::getFirst();
      while (element != nullptr)
      {
        element->refresh();
        element = element->getNext();
      }
    }
    {
      MIDI_Input_Element_ChannelPressure *element = MIDI_Input_Element_ChannelPressure::getFirst();
      while (element != nullptr)
      {
        element->refresh();
        element = element->getNext();
      }
    }
  }
  void refreshBankSelectors()
  {
    BankSelector *element = BankSelector::getFirst();
    while (element != nullptr)
    {
      element->refresh();
      element = element->getNext();
    }
  }
};

MIDI_Controller &MIDI_ControllerInstance = MIDI_Controller::getInstance();

#endif // MIDIctrl_h_
