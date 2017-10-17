#include "Control_Surface_Class.h"
#include "./MIDI_Outputs/MIDI_Control_Element.h"
#include "./Banks/BankSelector.h"

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
    refreshBankSelectors(); // refresh all bank selectors

    while (midi->refresh())
        ;
#ifndef NO_MIDI_INPUT
    updateMidiInput();
    refreshInputs();
#endif
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

void Control_Surface_::refreshBankSelectors()
{
    for (BankSelector *element = BankSelector::getFirst(); element != nullptr; element = element->getNext())
        element->refresh();
}

#ifndef NO_MIDI_INPUT

void Control_Surface_::updateMidiInput()
{
    if (midi->available() == 0) // if there are no MIDI messages in the buffer
        return;
    do
    {
        uint8_t header = midi->getNextHeader(); // get the header of the next MIDI message
        if (!header)                            // if no header was found and the buffer is empty
            return;

        uint8_t messageType = header & 0xF0;
        uint8_t data1 = midi->read(); // this is allowed, because all messages have at least 1 data byte

        if (messageType == CC && data1 == 0x79) // Reset All Controllers
        {
#ifdef DEBUG
            Serial.println("Reset All Controllers");
#endif
            for (MIDI_Input_Element_CC *element = MIDI_Input_Element_CC::getFirst(); element != nullptr; element = element->getNext())
                element->reset();
            for (MIDI_Input_Element_ChannelPressure *element = MIDI_Input_Element_ChannelPressure::getFirst(); element != nullptr; element = element->getNext())
                element->reset();
        }
        else if (messageType == CC && data1 == 0x7B) // All Notes off
        {
#ifdef DEBUG
            Serial.println("All Notes Off");
#endif
            for (MIDI_Input_Element_Note *element = MIDI_Input_Element_Note::getFirst(); element != nullptr; element = element->getNext())
                element->reset();
        }
        else
        {
#ifdef DEBUG
            Serial.printf("New midi message:\t%02X %02X\r\n", header, data1);
#endif
            if (messageType == CC) // Control Change
                for (MIDI_Input_Element_CC *element = MIDI_Input_Element_CC::getFirst(); element != nullptr; element = element->getNext())
                {
                    if (element->update(header, data1))
                        break;
                }
            else if (messageType == NOTE_OFF || messageType == NOTE_ON) // Note
                for (MIDI_Input_Element_Note *element = MIDI_Input_Element_Note::getFirst(); element != nullptr; element = element->getNext())
                {
                    if (element->update(header, data1))
                        break;
                }
            else if (messageType == CHANNEL_PRESSURE) // Channel Pressure
                for (MIDI_Input_Element_ChannelPressure *element = MIDI_Input_Element_ChannelPressure::getFirst(); element != nullptr; element = element->getNext())
                {
                    if (element->update(header, data1))
                        break;
                }
            else if (header == SysExStart) // System Exclusive
            {
#ifdef DEBUG
                Serial.print("System Exclusive:");
#endif
                while (data1 != SysExEnd && midi->available() > 0) // TODO: any status byte should end SysEx (MIDI 1.0 Detailed Specification 4.2 p.29)
                {
#ifdef DEBUG
                    Serial.print(' ');
                    Serial.print(data1, HEX);
#endif
                    data1 = midi->read();
                }
#ifdef DEBUG
                if (midi->available() == 0 && data1 != SysExEnd)
                {
                    Serial.println("\r\nReached end of buffer without SysExEnd");
                }
                else
                {
                    Serial.println("\tSysExEnd");
                }
#endif
            }
        }
    } while (midi->available() > 0);
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
#endif // ifndef NO_MIDI_INPUT

Control_Surface_ &Control_Surface = Control_Surface_::getInstance();
