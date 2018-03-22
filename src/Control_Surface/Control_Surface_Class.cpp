#include "Control_Surface_Class.h"
#include "../MIDI_Outputs/MIDI_Control_Element.h"
#include "../MIDI_Inputs/MIDI_Input_Element.h"
#include "../Banks/Selector.h"
#include "../Helpers/StreamOut.h"

// public:

Control_Surface_ &Control_Surface_::getInstance()
{
    static Control_Surface_ instance;
    return instance;
}
Control_Surface_::~Control_Surface_()
{
    delete new_midi;
}

void Control_Surface_::begin()
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
void Control_Surface_::refresh()
{
    if (midi == nullptr)
        begin(); // make sure that midi != nullptr

    refreshControls();      // refresh all control elements (Analog, AnalogHiRes, Digital, DigitalLatch, RotaryEncoder)
    refreshSelectors(); // refresh all bank selectors

    updateMidiInput();
    refreshInputs();
}

MIDI_Interface *Control_Surface_::MIDI()
{
    return midi;
}

// private:

void Control_Surface_::refreshControls()
{
    for (MIDI_Control_Element *element = MIDI_Control_Element::getFirst(); element != nullptr; element = element->getNext())
        element->refresh();
}

void Control_Surface_::refreshSelectors()
{
    for (Selector *element = Selector::getFirst(); element != nullptr; element = element->getNext())
        element->refresh();
}

void Control_Surface_::updateMidiInput()
{
#ifdef DEBUG
//DEBUG << "updateMidiInput()" << endl;
#endif
    MIDI_read_t midiReadResult = midi->read();
    while (midiReadResult != NO_MESSAGE)
    {
#ifdef DEBUG
        if (midiReadResult != NO_MESSAGE)
            DEBUG << "after midi->read()" << endl
                  << "Result: " << midiReadResult << endl;
        DEBUG.flush();
#endif

        // return; // TODO

        if (midiReadResult == CHANNEL_MESSAGE)
        {
#ifdef DEBUG
            DEBUG << "MIDI Channel message received" << endl;
#endif
            MIDI_message_matcher midimsg(midi->getChannelMessage());

            if (midimsg.type == CC && midimsg.data1 == 0x79) // Reset All Controllers
            {
#ifdef DEBUG
                DEBUG << "Reset All Controllers" << endl;
#endif
                for (MIDI_Input_Element_CC *element = MIDI_Input_Element_CC::getFirst(); element != nullptr; element = element->getNext())
                    element->reset();
                for (MIDI_Input_Element_ChannelPressure *element = MIDI_Input_Element_ChannelPressure::getFirst(); element != nullptr; element = element->getNext())
                    element->reset();
            }
            else if (midimsg.type == CC && midimsg.data1 == 0x7B) // All Notes off
            {
#ifdef DEBUG
                DEBUG << "All Notes Off" << endl;
#endif
                for (MIDI_Input_Element_Note *element = MIDI_Input_Element_Note::getFirst(); element != nullptr; element = element->getNext())
                    element->reset();
            }
            else
            {
#ifdef DEBUG
                DEBUG << "New midi message:\t" << hex << midimsg.type << ' ' << midimsg.channel << ' ' << midimsg.data1 << ' ' << midimsg.data2 << dec << endl;
#endif
                if (midimsg.type == CC) // Control Change
                    for (MIDI_Input_Element_CC *element = MIDI_Input_Element_CC::getFirst(); element != nullptr; element = element->getNext())
                    {
#ifdef DEBUG
                        DEBUG << "updating CC elements" << endl;
#endif
                        if (element->update(midimsg))
                            break;
                    }
                else if (midimsg.type == NOTE_OFF || midimsg.type == NOTE_ON) // Note
                    for (MIDI_Input_Element_Note *element = MIDI_Input_Element_Note::getFirst(); element != nullptr; element = element->getNext())
                    {
#ifdef DEBUG
                        DEBUG << "updating note elements" << endl;
#endif

                        if (element->update(midimsg))
                            break;
                    }
                else if (midimsg.type == CHANNEL_PRESSURE) // Channel Pressure
                    for (MIDI_Input_Element_ChannelPressure *element = MIDI_Input_Element_ChannelPressure::getFirst(); element != nullptr; element = element->getNext())
                    {
#ifdef DEBUG
                        DEBUG << "updating channel pressure elements" << endl;
#endif
                        if (element->update(midimsg))
                            break;
                    }
            }
        }
        else if (midiReadResult == SYSEX_MESSAGE) // System Exclusive
        {
#ifdef DEBUG
            DEBUG << "System Exclusive:" << tab;
            const uint8_t *data = midi->getSysExBuffer();
            size_t len = midi->getSysExLength();
            for (size_t i = 0; i < len; i++)
            {
                DEBUG << hex << data[i] << ' ' << dec;
            }
            DEBUG << endl;
#endif
        }
        midiReadResult = midi->read();
    }
}
void Control_Surface_::refreshInputs()
{
    for (MIDI_Input_Element_CC *element = MIDI_Input_Element_CC::getFirst(); element != nullptr; element = element->getNext())
        element->refresh();
    for (MIDI_Input_Element_Note *element = MIDI_Input_Element_Note::getFirst(); element != nullptr; element = element->getNext())
        element->refresh();
    for (MIDI_Input_Element_ChannelPressure *element = MIDI_Input_Element_ChannelPressure::getFirst(); element != nullptr; element = element->getNext())
        element->refresh();
}

Control_Surface_ &Control_Surface = Control_Surface_::getInstance();
