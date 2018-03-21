#include "ExtendedInputOutput.h"
#include "ExtendedIOElement.h"

namespace ExtIO
{
// #define DEBUG

ExtendedIOElement *getIOElementOfPin(pin_t pin)
{
    for (ExtendedIOElement *el = ExtendedIOElement::getFirst(); el != nullptr; el = el->getNext())
        if (pin >= el->getStart() && pin < el->getEnd())
            return el;
    return nullptr;
}

void pinMode(pin_t pin, uint8_t mode)
{
#ifdef DEBUG
    Serial.print("ExtIO::pinMode(");
    Serial.print(pin);
    Serial.print(", ");
    Serial.print(mode);
    Serial.print(");\r\n");
#endif
    if (pin < NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS)
        ::pinMode(pin, mode);
    else
    {
        ExtendedIOElement *el = getIOElementOfPin(pin);
        if (el != nullptr)
            el->pinMode(pin - el->getStart(), mode);
    }
}
void pinMode(int pin, uint8_t mode)
{
    pinMode((pin_t)pin, mode);
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
    if (pin < NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS)
        ::digitalWrite(pin, val);
    else
    {
        ExtendedIOElement *el = getIOElementOfPin(pin);
        if (el != nullptr)
            el->digitalWrite(pin - el->getStart(), val);
    }
}
void digitalWrite(int pin, uint8_t val)
{
    digitalWrite((pin_t)pin, val);
}
// void digitalWrite(pin_t pin, bool val)
// {
//     digitalWrite(pin, (uint8_t) val);
// }
int digitalRead(pin_t pin)
{
#ifdef DEBUG_READ
    Serial.print("ExtIO::digitalRead(");
    Serial.print(pin);
    Serial.print(");\r\n");
#endif
    if (pin < NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS)
        return ::digitalRead(pin);
    else
    {
        ExtendedIOElement *el = getIOElementOfPin(pin);
        if (el != nullptr)
            return el->digitalRead(pin - el->getStart());
    }
    return 0;
}
int digitalRead(int pin)
{
    return digitalRead((pin_t)pin);
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
void shiftOut(int dataPin, int clockPin, uint8_t bitOrder, uint8_t val)
{
    shiftOut((pin_t)dataPin, (pin_t)clockPin, bitOrder, val);
}
analog_t analogRead(pin_t pin)
{
#ifdef DEBUG_READ
    Serial.print("ExtIO::analogRead(");
    Serial.print(pin);
    Serial.print(");\r\n");
#endif
    if (pin < NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS)
        return ::analogRead(pin);
    else
    {
        ExtendedIOElement *el = getIOElementOfPin(pin);
        if (el != nullptr)
            return el->analogRead(pin - el->getStart());
    }
    return 0;
}
};