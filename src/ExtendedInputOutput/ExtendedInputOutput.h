#ifndef EXTENDEDINPUTOUTPUT_H_
#define EXTENDEDINPUTOUTPUT_H_

#include "Arduino.h"
#include "ExtendedIOElement.h"

#define EXT_PIN(x) (x + NUM_DIGITAL_PINS)

struct ExtendedIOListNode
{
    ExtendedIOListNode *next;
    pin_t start;
    pin_t end;
    ExtendedIOElement *element = nullptr;
};

class ExtendedInputOutput
{
  public:
    void pinMode(pin_t pin, uint8_t mode);
    void digitalWrite(pin_t pin, uint8_t val);
    int digitalRead(pin_t pin);

    void add(ExtendedIOElement &IOel);

    // private:
    pin_t pinOffset = NUM_DIGITAL_PINS;

    ExtendedIOListNode *firstExtIOLN = nullptr;
    ExtendedIOListNode *lastExtIOLN = nullptr;
};

void ExtendedInputOutput::add(ExtendedIOElement &IOel)
{
    pin_t length = IOel.getLength();
    IOel.setOffset(pinOffset);

    if (firstExtIOLN == nullptr)
    {
        firstExtIOLN = new ExtendedIOListNode;
        firstExtIOLN->element = &IOel;
        firstExtIOLN->start = pinOffset;
        firstExtIOLN->end = pinOffset + length;
        firstExtIOLN->next = nullptr;
        lastExtIOLN = firstExtIOLN;
    }
    else
    {
        ExtendedIOListNode *newLN = new ExtendedIOListNode;
        newLN->element = &IOel;
        newLN->start = pinOffset;
        newLN->end = pinOffset + length;
        newLN->next = nullptr;
        lastExtIOLN->next = newLN;
        lastExtIOLN = newLN;
    }
    pinOffset += IOel.getLength();
}

void ExtendedInputOutput::pinMode(pin_t pin, uint8_t mode)
{
    if (pin < NUM_DIGITAL_PINS)
    {
        // ::pinMode(pin, mode);
        Serial.print("::pinMode(");
        Serial.print(pin);
        Serial.print(", ");
        Serial.print(mode);
        Serial.print(");\r\n");
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
        ::digitalWrite(pin, val);else
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
    // if (digitalReadExt != nullptr)
    //     return digitalReadExt(pin - NUM_DIGITAL_PINS);
    return 0;
}

ExtendedInputOutput ExtendedIO;

namespace ExtIO
{

void pinMode(pin_t pin, uint8_t mode)
{
    ExtendedIO.pinMode(pin, mode);
}
void digitalWrite(pin_t pin, uint8_t val)
{
    ExtendedIO.digitalWrite(pin, val);
}
};
#endif // EXTENDEDINPUTOUTPUT_H_