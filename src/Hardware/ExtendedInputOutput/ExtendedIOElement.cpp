#include "ExtendedIOElement.h"
#include <Arduino.h>
#include <Helpers/Error.hpp>
#include <Helpers/Helpers.hpp>

ExtendedIOElement::ExtendedIOElement(pin_t length)
    : length(length), start(offset), end(offset + length) {
    elements.append(this);
    offset = end;
}

ExtendedIOElement::~ExtendedIOElement() { elements.remove(this); }

void ExtendedIOElement::beginAll() {
    for (ExtendedIOElement &e : elements)
        e.begin();
}

pin_t ExtendedIOElement::pin(pin_t p) {
    if (p >= length) {
        static_assert(is_unsigned<pin_t>::value, 
                      "Error: pin_t should be an unsigned integer type");
        ERROR(F("Error: the pin number (")
                  << +p
                  << F(") is greater than the number of pins of this "
                       "ExtendedIOElement (")
                  << +length << ')',
              0x4567);
        return end - 1;
    }
    return p + start;
}

pin_t ExtendedIOElement::getEnd() { return end; }

pin_t ExtendedIOElement::getStart() { return start; }

DoublyLinkedList<ExtendedIOElement> &ExtendedIOElement::getAll() {
    return elements;
}

DoublyLinkedList<ExtendedIOElement> ExtendedIOElement::elements;

pin_t ExtendedIOElement::offset = NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS;
