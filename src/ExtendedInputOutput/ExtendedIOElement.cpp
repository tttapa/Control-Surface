#include "ExtendedIOElement.h"

pin_t ExtendedIOElement::pin(pin_t p)
{
    return p + start;
}

pin_t ExtendedIOElement::getEnd()
{
    return end;
}

pin_t ExtendedIOElement::getStart()
{
    return start;
}

ExtendedIOElement *ExtendedIOElement::getNext()
{
    return next;
}
ExtendedIOElement *ExtendedIOElement::getFirst()
{
    return first;
}

ExtendedIOElement *ExtendedIOElement::first = nullptr;
ExtendedIOElement *ExtendedIOElement::last = nullptr;
pin_t ExtendedIOElement::offset = NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS;
