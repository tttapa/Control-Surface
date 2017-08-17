#include "Bank.h"
#include "MIDI_Control_Element.h"

Bank::Bank() {}

Bank::~Bank()
{
    element_node *element = firstElement;
    while (element != nullptr)
    {
        element_node *next = element->next;
        delete element;
        element = next;
    }
}

void Bank::add(MIDI_Control_Element *element)
{
    if (firstElement == nullptr)
    {
        firstElement = new element_node;
        firstElement->element = element;
        firstElement->next = nullptr;
        lastElement = firstElement;
    }
    else
    {
        element_node *newElement = new element_node;
        newElement->element = element;
        newElement->next = nullptr;
        lastElement->next = newElement;
        lastElement = newElement;
    }
}

void Bank::refresh()
{
    element_node *element = firstElement;
    while (element != nullptr)
    {
        element->element->refresh();
        element = element->next;
    }
}

void Bank::setChannel(uint8_t channel)
{
    element_node *element = firstElement;
    while (element != nullptr)
    {
        element->element->setChannel(channel);
        element = element->next;
    }
}