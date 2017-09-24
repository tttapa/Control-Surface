#include "MIDI_Controller_Class.h"
#include "./MIDI_Outputs/MIDI_Control_Element.h"
#include "./Banks/BankSelector.h"

// public:

MIDI_Controller_ &MIDI_Controller_::getInstance()
{
    static MIDI_Controller_ instance;
    return instance;
}
MIDI_Controller_::~MIDI_Controller_()
{
    delete new_midi;
}

void MIDI_Controller_::begin()
{
    if (midi == nullptr) // if this is the first time that begin is executed
    {
        midi = MIDI_Interface::getDefault(); // use the default MIDI interface
        if (midi == nullptr)                 // if it doesn't exist
        {
            new_midi = new USBMIDI_Interface; // create a new USB MIDI interface
            midi = new_midi;
        }
        midi->begin(); // initialize the MIDI interface
    }
}
void MIDI_Controller_::refresh()
{
    if (midi == nullptr)
        begin(); // make sure that midi != nullptr

    refreshControls();      // refresh all control elements (Analog, AnalogHiRes, Digital, DigitalLatch, RotaryEncoder)
    refreshBankSelectors(); // refresh all bank selectors

    while (midi->refresh()) // discard all MIDI input data
        ;
}

MIDI_Interface *MIDI_Controller_::MIDI()
{
    return midi;
}

// private:

void MIDI_Controller_::refreshControls()
{
    for (MIDI_Control_Element *element = MIDI_Control_Element::getFirst(); element != nullptr; element = element->getNext())
        element->refresh();
}

void MIDI_Controller_::refreshBankSelectors()
{
    for (BankSelector *element = BankSelector::getFirst(); element != nullptr; element = element->getNext())
        element->refresh();
}

MIDI_Controller_ &MIDI_Controller = MIDI_Controller_::getInstance();
