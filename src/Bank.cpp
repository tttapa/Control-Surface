#include "Bank.h"
#include "MIDI_Control_Element.h"

Bank::Bank() {}

Bank::~Bank()
{
    bank_element *element = firstElement;
    while (element != nullptr)
    {
        bank_element *next = element->next;
        delete element;
        element = next;
    }
}

void Bank::add(MIDI_Control_Element *element, bankType type)
{
    bank_element *newElement = new bank_element;
    newElement->element = element;
    newElement->next = nullptr;
    newElement->type = type;
    if (firstElement == nullptr)
    {
        firstElement = newElement;
        lastElement = firstElement;
    }
    else
    {

        lastElement->next = newElement;
        lastElement = newElement;
    }
}
void Bank::add(MIDI_Control_Element &element, bankType type)
{
    add(&element, type);
}
void Bank::refresh()
{
    bank_element *element = firstElement;
    while (element != nullptr)
    {
        element->element->refresh();
        element = element->next;
    }
}

void Bank::setBankSetting(uint8_t bankSetting)
{
    bank_element *element = firstElement;
    while (element != nullptr)
    {
        if (element->type == CHANGE_CHANNEL)
            element->element->setChannelOffset(bankSetting);
        else
            element->element->setAddressOffset(bankSetting);
        element = element->next;
    }
}

void Bank::average(size_t length)
{
    bank_element *element = firstElement;
    while (element != nullptr)
    {
        element->element->average(length);
        element = element->next;
    }
}
