#include "Bank.h"
#include "MIDI_Interface.h"

Bank::Bank() {}

Bank::~Bank()
{
    control_element *element = firstControlElement;
    while (element != nullptr)
    {
        control_element *next = element->next;
        delete element;
        element = next;
    }
}

void Bank::add(MIDI_Control_Element *element, bankType type)
{
    control_element *newElement = new control_element;
    newElement->element = element;
    newElement->next = nullptr;
    newElement->type = type;
    if (firstControlElement == nullptr)
    {
        firstControlElement = newElement;
        lastControlElement = firstControlElement;
    }
    else
    {

        lastControlElement->next = newElement;
        lastControlElement = newElement;
    }
}
void Bank::add(MIDI_Control_Element &element, bankType type)
{
    add(&element, type);
}

void Bank::add(MIDI_Input_Element *element, bankType type)
{
    input_element *newElement = new input_element;
    newElement->element = element;
    newElement->next = nullptr;
    newElement->type = type;
    if (firstInputElement == nullptr)
    {
        firstInputElement = newElement;
        lastInputElement = firstInputElement;
    }
    else
    {

        lastInputElement->next = newElement;
        lastInputElement = newElement;
    }
}
void Bank::add(MIDI_Input_Element &element, bankType type)
{
    add(&element, type);
}

void Bank::refresh()
{
    control_element *element = firstControlElement;
    while (element != nullptr)
    {
        element->element->refresh();
        element = element->next;
    }
    refreshMIDI();
    updateMidiInput();
}

void Bank::setBankSetting(uint8_t bankSetting)
{
    {
        control_element *element = firstControlElement;
        while (element != nullptr)
        {
            if (element->type == CHANGE_CHANNEL)
                element->element->setChannelOffset(bankSetting);
            else
                element->element->setAddressOffset(bankSetting);
            element = element->next;
        }
    }
    {
        input_element *element = firstInputElement;
        while (element != nullptr)
        {
            if (element->type == CHANGE_CHANNEL)
                element->element->setChannelOffset(bankSetting);
            else
                element->element->setAddressOffset(bankSetting);
            element = element->next;
        }
    }
}

void Bank::average(size_t length)
{
    control_element *element = firstControlElement;
    while (element != nullptr)
    {
        element->element->average(length);
        element = element->next;
    }
}

void Bank::updateMidiInput()
{
    if (availableMIDI() == 0)
        return;
    do
    {
        MIDI_event *midimsg = readMIDI();
        Serial.print("New midi message:\t");
        Serial.printf("%02X %02X %02x\r\n", midimsg->header, midimsg->data1, midimsg->data2);
        // if (((midimsg->header & 0b11110000) == NOTE_ON) || ((midimsg->header & 0b11110000) == NOTE_OFF))
        // {
        for (input_element *node = firstInputElement; node != nullptr; node = node->next)
        {
            if (node->element->update(midimsg->header, midimsg->data1, midimsg->data2))
            {
                Serial.println("\tMatch");
                break;
            }
        }
        //  }
    } while (availableMIDI() > 0);
}