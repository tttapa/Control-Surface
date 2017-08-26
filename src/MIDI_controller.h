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
      MIDI_event *midimsg = readMIDI();

      Serial.print("New midi message:\t");
      Serial.printf("%02X %02X %02x\r\n", midimsg->header, midimsg->data1, midimsg->data2);

      for (MIDI_Input_Element *element = MIDI_Input_Element::getFirst(); element != nullptr; element = element->getNext())
      {
        if (element->update(midimsg->header, midimsg->data1, midimsg->data2))
        {
          Serial.println("\tMatch");
          break;
        }
      }
    } while (availableMIDI() > 0);
  }
  void refreshInputs()
  {
    MIDI_Input_Element *element = MIDI_Input_Element::getFirst();
    while (element != nullptr)
    {
      element->refresh();
      element = element->getNext();
    }
  }
};

MIDI_Controller& MIDI_ControllerInstance = MIDI_Controller::getInstance();

#endif // MIDIctrl_h_
