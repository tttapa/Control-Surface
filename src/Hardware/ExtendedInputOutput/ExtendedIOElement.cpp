#include "ExtendedIOElement.h"
#include <Arduino.h>

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

pin_t ExtendedIOElement::pin(pin_t p) { return p + start; }

pin_t ExtendedIOElement::getEnd() { return end; }

pin_t ExtendedIOElement::getStart() { return start; }

DoublyLinkedList<ExtendedIOElement> &ExtendedIOElement::getAll() {
    return elements;
}

DoublyLinkedList<ExtendedIOElement> ExtendedIOElement::elements;

pin_t ExtendedIOElement::offset = NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS;
