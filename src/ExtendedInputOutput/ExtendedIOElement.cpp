#include "ExtendedIOElement.h"

ExtendedIOElement *ExtendedIOElement::first = nullptr;
ExtendedIOElement *ExtendedIOElement::last = nullptr;
pin_t ExtendedIOElement::offset = NUM_DIGITAL_PINS;