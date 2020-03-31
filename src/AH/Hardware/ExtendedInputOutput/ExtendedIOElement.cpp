#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include "ExtendedIOElement.hpp"
#include <AH/Error/Error.hpp>
#include <AH/STL/type_traits> // is_unsigned

BEGIN_AH_NAMESPACE

ExtendedIOElement::ExtendedIOElement(pin_t length)
    : length(length), start(offset), end(offset + length) {
    if (end < start)
        FATAL_ERROR(F("ExtIO ran out of pin numbers. "
                      "Dynamically creating new ExtendedIOElements is not "
                      "recommended."),
                    0x00FF);
    offset = end;
}

void ExtendedIOElement::beginAll() {
    ExtendedIOElement::applyToAll(&ExtendedIOElement::begin);
}

void ExtendedIOElement::updateAllBufferedOutputs() {
    ExtendedIOElement::applyToAll(&ExtendedIOElement::updateBufferedOutputs);
}

void ExtendedIOElement::updateAllBufferedInputs() {
    ExtendedIOElement::applyToAll(&ExtendedIOElement::updateBufferedInputs);
}

pin_t ExtendedIOElement::pin(pin_t p) const {
    if (p >= length) {
        static_assert(std::is_unsigned<pin_t>::value,
                      "Error: pin_t should be an unsigned integer type");
        ERROR(F("Error: the pin number (")
                  << p
                  << F(") is greater than the number of pins of this "
                       "ExtendedIOElement (")
                  << length << ')',
              0x4567);
        return end - 1; // LCOV_EXCL_LINE
    }
    return p + start;
}

pin_t ExtendedIOElement::operator[](pin_t p) const { return pin(p); }

pin_t ExtendedIOElement::getLength() const { return length; }

pin_t ExtendedIOElement::getEnd() const { return end; }

pin_t ExtendedIOElement::getStart() const { return start; }

DoublyLinkedList<ExtendedIOElement> &ExtendedIOElement::getAll() {
    return updatables;
}

pin_t ExtendedIOElement::offset = NUM_DIGITAL_PINS + NUM_ANALOG_INPUTS;

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()