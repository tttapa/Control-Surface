#include "RelativeCCOut.h"
#include "../../Control_Surface/Control_Surface_Class.h"

RelativeCCOut::RelativeCCOut(uint8_t controllerNumber, uint8_t channel,
                             relativeCCmode mode)
    : controllerNumber(controllerNumber), channel(channel), mode(mode) {}

uint8_t RelativeCCOut::twosComplement7bit(int8_t value) {
    return value & 0b01111111;
}

uint8_t RelativeCCOut::twosComplementTo7bitSignedMagnitude(int8_t value) {
    uint8_t mask = value >> 7;
    uint8_t abs = (value + mask) ^ mask;
    uint8_t sign = mask & 0b01000000;
    return (abs & 0b00111111) | sign;
}

uint8_t RelativeCCOut::mapRelativeCC(int8_t value, relativeCCmode mode) {
    switch (mode) {
    case TWOS_COMPLEMENT:
        return twosComplement7bit(value);
    case BINARY_OFFSET:
        return value + 64;
    case SIGN_MAGNITUDE:
        return twosComplementTo7bitSignedMagnitude(value);
    }
}

void RelativeCCOut::send(long delta) {
    while (delta != 0) {
        // Constrain relative movement to +/-15 for
        // Mackie Control Universal compatibility
        long thisDelta = constrain(delta, -15, 15);
        uint8_t msgVal = mapRelativeCC(thisDelta, mode);
        Control_Surface.MIDI().send(
            CONTROL_CHANGE, // send a Control Change MIDI event
            channel + channelOffset * tracksPerBank,
            controllerNumber + addressOffset * tracksPerBank, msgVal);
        delta -= thisDelta;
    }
}

void RelativeCCOut::update() { refresh(); }