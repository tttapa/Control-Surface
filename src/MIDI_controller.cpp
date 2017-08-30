#include "MIDI_controller.h"

#include "MIDI_Interface.h"
#include "USBMIDI_Interface.h"

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
    }
    midi->begin(); // initialize the MIDI interface
}
void MIDI_Controller_::refresh()
{
    refreshControls();
    refreshBankSelectors();
    while (refreshMIDI())
        ;
    updateMidiInput();
    refreshInputs();
}

MIDI_Interface *MIDI_Controller_::MIDI()
{
    if (midi == nullptr)
        begin();
    return midi;
}

// private:

void MIDI_Controller_::refreshControls()
{
    for (MIDI_Control_Element *element = MIDI_Control_Element::getFirst(); element != nullptr; element = element->getNext())
        element->refresh();
}

void MIDI_Controller_::updateMidiInput()
{
    if (midi->available() == 0) // if there are no MIDI messages in the buffer
        return;
    do
    {
        uint8_t header = midi->getNextHeader(); // get the header of the next MIDI message
        if (!header)                            // if no header was found and the buffer is empty
            return;

        uint8_t messageType = header & 0xF0;
        uint8_t targetChannel = header & 0x0F;
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
                    if (element->update(data1, targetChannel)) // TODO parameter order
                        break;
                }
            else if (messageType == NOTE_OFF || messageType == NOTE_ON) // Note
                for (MIDI_Input_Element_Note *element = MIDI_Input_Element_Note::getFirst(); element != nullptr; element = element->getNext())
                {
                    if (element->update(messageType, data1, targetChannel))
                        break;
                }
            else if (messageType == CHANNEL_PRESSURE) // Channel Pressure
                for (MIDI_Input_Element_ChannelPressure *element = MIDI_Input_Element_ChannelPressure::getFirst(); element != nullptr; element = element->getNext())
                {
                    if (element->update(targetChannel, data1)) // TODO parameter order
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
void MIDI_Controller_::refreshInputs()
{
    for (MIDI_Input_Element_CC *element = MIDI_Input_Element_CC::getFirst(); element != nullptr; element = element->getNext())
        element->refresh();
    for (MIDI_Input_Element_Note *element = MIDI_Input_Element_Note::getFirst(); element != nullptr; element = element->getNext())
        element->refresh();
    for (MIDI_Input_Element_ChannelPressure *element = MIDI_Input_Element_ChannelPressure::getFirst(); element != nullptr; element = element->getNext())
        element->refresh();
}
void MIDI_Controller_::refreshBankSelectors()
{
    for (BankSelector *element = BankSelector::getFirst(); element != nullptr; element = element->getNext())
        element->refresh();
}

MIDI_Controller_ &MIDI_Controller = MIDI_Controller_::getInstance();
