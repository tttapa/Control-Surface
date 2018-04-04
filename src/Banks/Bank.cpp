#include "Bank.h"

Bank::Bank(uint8_t tracksPerBank) : tracksPerBank(tracksPerBank) {}

Bank::~Bank()
{
    MIDI_Element_list_node *element = firstMIDI_Element;
    while (element != nullptr)
    {
        MIDI_Element_list_node *next = element->next;
        delete element;
        element = next;
    }
}

void Bank::add(MIDI_Element *element, bankType type)
{
    element->settracksPerBank(tracksPerBank);

    MIDI_Element_list_node *newElement = new MIDI_Element_list_node;
    newElement->element = element;
    newElement->next = nullptr;
    newElement->type = type;
    if (firstMIDI_Element == nullptr)
        firstMIDI_Element = newElement;
    else
        lastMIDI_Element->next = newElement;
    lastMIDI_Element = newElement;
}

void Bank::add(MIDI_Element &element, bankType type)
{
    add(&element, type);
}

void Bank::setBankSetting(uint8_t bankSetting)
{
    MIDI_Element_list_node *element = firstMIDI_Element;
    while (element != nullptr)
    {
        if (element->type == CHANGE_CHANNEL)
            element->element->setChannelOffset(bankSetting);
        else
            element->element->setAddressOffset(bankSetting);
        element = element->next;
    }
}

void Bank::map(int (*fn)(int))
{
    MIDI_Element_list_node *element = firstMIDI_Element;
    while (element != nullptr)
    {
        element->element->map(fn);
        element = element->next;
    }
}
