#include "Control_Surface_Class.h"
#include "../MIDI_Outputs/MIDI_Control_Element.h"
#include "../MIDI_Inputs/MIDI_Input_Element.h"
#include "../Banks/BankSelector.h"
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
#if defined(DEBUG) || defined(DEBUG_MIDI_PACKETS)
    DEBUG_OUT.begin(115200);
#endif
}
void Control_Surface_::refresh()
{
    if (midi == nullptr)
        begin(); // make sure that midi != nullptr

    refreshControls();  // refresh all control elements
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
//DEBUG_OUT << "updateMidiInput()" << endl;
#endif
    MIDI_read_t midiReadResult = midi->read();
    while (midiReadResult != NO_MESSAGE)
    {
#ifdef DEBUG
        if (midiReadResult != NO_MESSAGE)
            DEBUG_OUT << "after midi->read()" << endl
                      << "Result: " << midiReadResult << endl;
        DEBUG_OUT.flush();
#endif

        // return; // TODO

        if (midiReadResult == CHANNEL_MESSAGE)
        {
            MIDI_message_matcher midimsg(midi->getChannelMessage());

#ifdef DEBUG_MIDI_PACKETS
            DEBUG_OUT << ">> " << hex << midi->getChannelMessage().header << ' ' << midimsg.data1;
            if (midimsg.type != PROGRAM_CHANGE && midimsg.type != CHANNEL_PRESSURE)
            {
                DEBUG_OUT << ' ' << midimsg.data2;
            }
            DEBUG_OUT << dec;
            if (midimsg.type == CHANNEL_PRESSURE)
                DEBUG_OUT << tab << millis();
            DEBUG_OUT << endl;
#endif
#ifdef DEBUG
            DEBUG_OUT << "MIDI Channel message received" << endl;
#endif
            if (midimsg.type == CC && midimsg.data1 == 0x79) // Reset All Controllers
            {
#ifdef DEBUG
                DEBUG_OUT << "Reset All Controllers" << endl;
#endif
                for (MIDI_Input_Element_CC *element = MIDI_Input_Element_CC::getFirst(); element != nullptr; element = element->getNext())
                    element->reset();
                for (MIDI_Input_Element_ChannelPressure *element = MIDI_Input_Element_ChannelPressure::getFirst(); element != nullptr; element = element->getNext())
                    element->reset();
            }
            else if (midimsg.type == CC && midimsg.data1 == 0x7B) // All Notes off
            {
#ifdef DEBUG
                DEBUG_OUT << "All Notes Off" << endl;
#endif
                for (MIDI_Input_Element_Note *element = MIDI_Input_Element_Note::getFirst(); element != nullptr; element = element->getNext())
                    element->reset();
            }
            else
            {
#ifdef DEBUG
                DEBUG_OUT << "New midi message:\t" << hex << midimsg.type << ' ' << midimsg.channel << ' ' << midimsg.data1 << ' ' << midimsg.data2 << dec << endl;
#endif
                if (midimsg.type == CC) // Control Change
                    for (MIDI_Input_Element_CC *element = MIDI_Input_Element_CC::getFirst(); element != nullptr; element = element->getNext())
                    {
#ifdef DEBUG
                        DEBUG_OUT << "updating CC elements" << endl;
#endif
                        if (element->update(midimsg))
                            break;
                    }
                else if (midimsg.type == NOTE_OFF || midimsg.type == NOTE_ON) // Note
                    for (MIDI_Input_Element_Note *element = MIDI_Input_Element_Note::getFirst(); element != nullptr; element = element->getNext())
                    {
#ifdef DEBUG
                        DEBUG_OUT << "updating note elements" << endl;
#endif

                        if (element->update(midimsg))
                            break;
                    }
                else if (midimsg.type == CHANNEL_PRESSURE) // Channel Pressure
                    for (MIDI_Input_Element_ChannelPressure *element = MIDI_Input_Element_ChannelPressure::getFirst(); element != nullptr; element = element->getNext())
                    {
#ifdef DEBUG
                        DEBUG_OUT << "updating channel pressure elements" << endl;
#endif
                        if (element->update(midimsg))
                            break;
                    }
            }
        }
        else if (midiReadResult == SYSEX_MESSAGE) // System Exclusive
        {
#ifdef DEBUG
            DEBUG_OUT << "System Exclusive:" << tab;
            const uint8_t *data = midi->getSysExBuffer();
            size_t len = midi->getSysExLength();
            for (size_t i = 0; i < len; i++)
            {
                DEBUG_OUT << hex << data[i] << ' ' << dec;
            }
            DEBUG_OUT << endl;
#endif
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
