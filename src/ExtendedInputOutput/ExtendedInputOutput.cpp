#include "ExtendedInputOutput.h"

void ExtendedInputOutput::add(ExtendedIOElement &IOel)
{
    add(&IOel);
}

void ExtendedInputOutput::add(ExtendedIOElement *IOel)
{
    pin_t length = IOel->getLength();
    IOel->setOffset(pinOffset);

    if (firstExtIOLN == nullptr)
    {
        firstExtIOLN = new ExtendedIOListNode;
        firstExtIOLN->element = IOel;
        firstExtIOLN->start = pinOffset;
        firstExtIOLN->end = pinOffset + length;
        firstExtIOLN->next = nullptr;
        lastExtIOLN = firstExtIOLN;
    }
    else
    {
        ExtendedIOListNode *newLN = new ExtendedIOListNode;
        newLN->element = IOel;
        newLN->start = pinOffset;
        newLN->end = pinOffset + length;
        newLN->next = nullptr;
        lastExtIOLN->next = newLN;
        lastExtIOLN = newLN;
    }
    pinOffset += IOel->getLength();
}

void ExtendedInputOutput::pinMode(pin_t pin, uint8_t mode)
{
    if (pin < NUM_DIGITAL_PINS)
    {
        ::pinMode(pin, mode);
#ifdef DEBUG
        Serial.print("::pinMode(");
        Serial.print(pin);
        Serial.print(", ");
        Serial.print(mode);
        Serial.print(");\r\n");
#endif
    }
    else
    {
        ExtendedIOListNode *ListNode = firstExtIOLN;
        while (ListNode != nullptr)
        {
            if (pin >= ListNode->start && pin < ListNode->end)
            {
                if (ListNode->element != nullptr)
                    ListNode->element->pinMode(pin - ListNode->start, mode);
                return;
            }
            ListNode = ListNode->next;
        }
    }
}
void ExtendedInputOutput::digitalWrite(pin_t pin, uint8_t val)
{
    if (pin < NUM_DIGITAL_PINS)
        ::digitalWrite(pin, val);
    else
    {
        ExtendedIOListNode *ListNode = firstExtIOLN;
        while (ListNode != nullptr)
        {
            if (pin >= ListNode->start && pin < ListNode->end)
            {
                if (ListNode->element != nullptr)
                    ListNode->element->digitalWrite(pin - ListNode->start, val);
                return;
            }
            ListNode = ListNode->next;
        }
    }
}
int ExtendedInputOutput::digitalRead(pin_t pin)
{
    if (pin < NUM_DIGITAL_PINS)
        return ::digitalRead(pin);

    ExtendedIOListNode *ListNode = firstExtIOLN;
    while (ListNode != nullptr)
    {
        if (pin >= ListNode->start && pin < ListNode->end)
        {
            if (ListNode->element != nullptr)
                return ListNode->element->digitalRead(pin - ListNode->start);
        }
        ListNode = ListNode->next;
    }
    return 0;
}

ExtendedInputOutput ExtendedIO;

namespace ExtIO
{
// #define DEBUG

void pinMode(pin_t pin, uint8_t mode)
{
#ifdef DEBUG
    Serial.print("ExtIO::pinMode(");
    Serial.print(pin);
    Serial.print(", ");
    Serial.print(mode);
    Serial.print(");\r\n");
#endif
    ExtendedIO.pinMode(pin, mode);
}
void digitalWrite(pin_t pin, uint8_t val)
{
#ifdef DEBUG
    Serial.print("ExtIO::digitalWrite(");
    Serial.print(pin);
    Serial.print(", ");
    Serial.print(val);
    Serial.print(");\r\n");
#endif
    ExtendedIO.digitalWrite(pin, val);
}
int digitalRead(pin_t pin)
{
#ifdef DEBUG
    Serial.print("ExtIO::digitalRead(");
    Serial.print(pin);
    Serial.print(");\r\n");
#endif
    return ExtendedIO.digitalRead(pin);
}
void shiftOut(pin_t dataPin, pin_t clockPin, uint8_t bitOrder, uint8_t val)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        if (bitOrder == LSBFIRST)
            digitalWrite(dataPin, !!(val & (1 << i)));
        else
            digitalWrite(dataPin, !!(val & (1 << (7 - i))));

        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
    }
}
}