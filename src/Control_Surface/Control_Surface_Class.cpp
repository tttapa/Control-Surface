#include "Control_Surface_Class.h"
#include "../MIDI_Outputs/MIDI_Control_Element.h"
#include "../MIDI_Inputs/MIDI_Input_Element.h"
#include "../Banks/BankSelector.h"
#include "../Helpers/StreamOut.h"
#include "../Helpers/PerfCounter.h"

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
#if defined(DEBUG) || defined(DEBUG_MIDI_PACKETS) || defined(DEBUG_TIME)
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
#ifdef DEBUG_TIME
    PerfCounter::print();
#endif
}

MIDI_Interface *Control_Surface_::MIDI()
{
    return midi;
}

// private:

void Control_Surface_::refreshControls()
{
    static PerfCounter pc("Refreshing control elements");
    pc.start();
    for (MIDI_Control_Element *element = MIDI_Control_Element::getFirst(); element != nullptr; element = element->getNext())
        element->refresh();
    pc.stop();
}

void Control_Surface_::refreshSelectors()
{
    static PerfCounter pc("Refreshing selectors");
    pc.start();
    for (Selector *element = Selector::getFirst(); element != nullptr; element = element->getNext())
        element->refresh();
    pc.stop();
}

void Control_Surface_::updateMidiInput()
{
    static PerfCounter pc("Updating MIDI input");
    pc.start();
#ifdef DEBUG
//DEBUG_OUT << "updateMidiInput()" << endl;
#endif
    static PerfCounter pcmr("\tMIDI read");
    pcmr.start();
    MIDI_read_t midiReadResult = midi->read();
    // pcmr.stop();
#ifdef DEBUG_TIME
    // DEBUG_OUT << "midiReadResult = " << midiReadResult << endl;
#endif
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
                static PerfCounter pcreset("\t\tResetting CC and Channel Pressure input elements");
                pcreset.start();
                for (MIDI_Input_Element_CC *element = MIDI_Input_Element_CC::getFirst(); element != nullptr; element = element->getNext())
                    element->reset();
                for (MIDI_Input_Element_ChannelPressure *element = MIDI_Input_Element_ChannelPressure::getFirst(); element != nullptr; element = element->getNext())
                    element->reset();
                pcreset.stop();
            }
            else if (midimsg.type == CC && midimsg.data1 == 0x7B) // All Notes off
            {
#ifdef DEBUG
                DEBUG_OUT << "All Notes Off" << endl;
#endif
                static PerfCounter pcnoteoff("\t\tTurning all notes off");
                pcnoteoff.start();
                for (MIDI_Input_Element_Note *element = MIDI_Input_Element_Note::getFirst(); element != nullptr; element = element->getNext())
                    element->reset();
                pcnoteoff.stop();
            }
            else
            {
#ifdef DEBUG
                DEBUG_OUT << "New midi message:\t" << hex << midimsg.type << ' ' << midimsg.channel << ' ' << midimsg.data1 << ' ' << midimsg.data2 << dec << endl;
#endif
                if (midimsg.type == CC) // Control Change
                {
                    static PerfCounter pcCC("\t\tUpdating and matching CC");
                    pcCC.start();
                    for (MIDI_Input_Element_CC *element = MIDI_Input_Element_CC::getFirst(); element != nullptr; element = element->getNext())
                    {
#ifdef DEBUG
                        DEBUG_OUT << "updating CC elements" << endl;
#endif
                        if (element->update(midimsg))
                            break;
                    }
                    pcCC.stop();
                }
                else if (midimsg.type == NOTE_OFF || midimsg.type == NOTE_ON) // Note
                {
                    static PerfCounter pcN("\t\tUpdating and matching Note");
                    pcN.start();
                    for (MIDI_Input_Element_Note *element = MIDI_Input_Element_Note::getFirst(); element != nullptr; element = element->getNext())
                    {
#ifdef DEBUG
                        DEBUG_OUT << "updating note elements" << endl;
#endif

                        if (element->update(midimsg))
                            break;
                    }
                    pcN.stop();
                }
                else if (midimsg.type == CHANNEL_PRESSURE) // Channel Pressure
                {
                    static PerfCounter pcCP("\t\tUpdating and matching Channel Pressure");
                    pcCP.start();
                    for (MIDI_Input_Element_ChannelPressure *element = MIDI_Input_Element_ChannelPressure::getFirst(); element != nullptr; element = element->getNext())
                    {
#ifdef DEBUG
                        DEBUG_OUT << "updating channel pressure elements" << endl;
#endif
                        if (element->update(midimsg))
                            break;
                    }
                    pcCP.stop();
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
        pcmr.start();
        midiReadResult = midi->read();
        pcmr.stop();
    }
    pc.stop();
}
void Control_Surface_::refreshInputs()
{
    static PerfCounter pcCC("Refreshing input elements CC");
    pcCC.start();
    for (MIDI_Input_Element_CC *element = MIDI_Input_Element_CC::getFirst(); element != nullptr; element = element->getNext())
        element->refresh();
    pcCC.stop();
    static PerfCounter pcN("Refreshing input elements Note");
    pcN.start();
    for (MIDI_Input_Element_Note *element = MIDI_Input_Element_Note::getFirst(); element != nullptr; element = element->getNext())
        element->refresh();
    pcN.stop();
    static PerfCounter pcCP("Refreshing input elements Channel Pressure");
    pcCP.start();
    for (MIDI_Input_Element_ChannelPressure *element = MIDI_Input_Element_ChannelPressure::getFirst(); element != nullptr; element = element->getNext())
        element->refresh();
    pcCP.stop();
}

Control_Surface_ &Control_Surface = Control_Surface_::getInstance();
