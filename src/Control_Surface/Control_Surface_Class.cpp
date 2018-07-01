#include "Control_Surface_Class.h"
#include "../MIDI_Outputs/MIDI_Control_Element.h"
#include "../MIDI_Inputs/MIDI_Input_Element.h"
#include "../Banks/BankSelector.h"

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
#if defined(DEBUG_OUT)
    DEBUG_OUT.begin(115200);
#endif
}

void Control_Surface_::update()
{
    if (midi == nullptr)
        begin(); // make sure that midi != nullptr

    updateControls();  // update all control elements
    updateSelectors(); // update all bank selectors

    updateMidiInput();
    updateInputs();
    updateDisplays();
}

MIDI_Interface *Control_Surface_::MIDI()
{
    return midi;
}

void Control_Surface_::updateControls()
{
    for (MIDI_Control_Element *element = MIDI_Control_Element::getFirst(); element != nullptr; element = element->getNext())
        element->update();
}

void Control_Surface_::updateSelectors()
{
    for (Selector *element = Selector::getFirst(); element != nullptr; element = element->getNext())
        element->update();
}

void Control_Surface_::updateMidiInput()
{
    MIDI_read_t midiReadResult = midi->read();

    while (midiReadResult != NO_MESSAGE)
    {
        if (midiReadResult == CHANNEL_MESSAGE)
        {
            MIDI_message_matcher midimsg(midi->getChannelMessage());

#ifdef DEBUG_MIDI_PACKETS
            if (midimsg.type != PROGRAM_CHANGE && midimsg.type != CHANNEL_PRESSURE)
                DEBUG(">>> " << hex << midi->getChannelMessage().header << ' ' << midimsg.data1 << ' ' << midimsg.data2);
            else
                DEBUG(">>> " << hex << midi->getChannelMessage().header << ' ' << midimsg.data1);
#endif
            if (midimsg.type == CC && midimsg.data1 == 0x79) // Reset All Controllers
            {
                DEBUG("Reset All Controllers");
                for (MIDI_Input_Element_CC *element = MIDI_Input_Element_CC::getFirst(); element != nullptr; element = element->getNext())
                    element->reset();
                for (MIDI_Input_Element_ChannelPressure *element = MIDI_Input_Element_ChannelPressure::getFirst(); element != nullptr; element = element->getNext())
                    element->reset();
            }
            else if (midimsg.type == CC && midimsg.data1 == 0x7B) // All Notes off
            {
                for (MIDI_Input_Element_Note *element = MIDI_Input_Element_Note::getFirst(); element != nullptr; element = element->getNext())
                    element->reset();
            }
            else
            {
                if (midimsg.type == CC) // Control Change
                {
                    for (MIDI_Input_Element_CC *element = MIDI_Input_Element_CC::getFirst(); element != nullptr; element = element->getNext())
                    {
                        if (element->updateWith(midimsg))
                            break;
                    }
                }
                else if (midimsg.type == NOTE_OFF || midimsg.type == NOTE_ON) // Note
                {
                    for (MIDI_Input_Element_Note *element = MIDI_Input_Element_Note::getFirst(); element != nullptr; element = element->getNext())
                    {
                        if (element->updateWith(midimsg))
                            break;
                    }
                }
                else if (midimsg.type == CHANNEL_PRESSURE) // Channel Pressure
                {
                    for (MIDI_Input_Element_ChannelPressure *element = MIDI_Input_Element_ChannelPressure::getFirst(); element != nullptr; element = element->getNext())
                    {
                        if (element->updateWith(midimsg))
                            break;
                    }
                }
            }
        }
        else if (midiReadResult == SYSEX_MESSAGE) // System Exclusive
        {
#ifdef DEBUG_MIDI_PACKETS
            const uint8_t *data = midi->getSysExBuffer();
            size_t len = midi->getSysExLength();
            for (size_t i = 0; i < len; i++)
            {
                DEBUG_OUT << hex << data[i] << ' ';
            }
            DEBUG_OUT << dec << endl;
#endif
        }
        midiReadResult = midi->read();
    }
}

void Control_Surface_::updateInputs()
{
    for (MIDI_Input_Element_CC *element = MIDI_Input_Element_CC::getFirst(); element != nullptr; element = element->getNext())
        element->update();
    for (MIDI_Input_Element_Note *element = MIDI_Input_Element_Note::getFirst(); element != nullptr; element = element->getNext())
        element->update();
    for (MIDI_Input_Element_ChannelPressure *element = MIDI_Input_Element_ChannelPressure::getFirst(); element != nullptr; element = element->getNext())
        element->update();
}

Control_Surface_ &Control_Surface = Control_Surface_::getInstance();
