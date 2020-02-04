#include "MIDIAddress.hpp"

BEGIN_CS_NAMESPACE

MIDIAddress &MIDIAddress::
operator+=(const RelativeMIDIAddress &rhs) {
    if (!this->isValid() || !rhs.isValid()) {
        this->addresses.valid = false;
    } else {
        this->addresses.address += rhs.addresses.address;
        this->addresses.channel += rhs.addresses.channel;
        this->addresses.cableNumber += rhs.addresses.cableNumber;
    }
    return *this;
}

MIDIAddress &MIDIAddress::
operator-=(const RelativeMIDIAddress &rhs) {
    if (!this->isValid() || !rhs.isValid()) {
        this->addresses.valid = false;
    } else {
        this->addresses.address -= rhs.addresses.address;
        this->addresses.channel -= rhs.addresses.channel;
        this->addresses.cableNumber -= rhs.addresses.cableNumber;
    }
    return *this;
}

MIDIAddress MIDIAddress::
operator+(const RelativeMIDIAddress &rhs) const {
    MIDIAddress copy = *this;
    copy += rhs;
    return copy;
}

MIDIAddress MIDIAddress::
operator-(const RelativeMIDIAddress &rhs) const {
    MIDIAddress copy = *this;
    copy -= rhs;
    return copy;
}

bool MIDIAddress::matchSingle(const MIDIAddress &toMatch,
                                       const MIDIAddress &base) {
    return base == toMatch;
}

bool MIDIAddress::matchAddressInRange(
    const MIDIAddress &toMatch, const MIDIAddress &base,
    uint8_t length) {
    bool valid = base.addresses.valid && toMatch.addresses.valid;
    bool addressInRange =
        base.addresses.address <= toMatch.addresses.address &&
        base.addresses.address + length > toMatch.addresses.address;
    bool equalChannelAndCN =
        base.addresses.channel == toMatch.addresses.channel &&
        base.addresses.cableNumber == toMatch.addresses.cableNumber;
    return valid && addressInRange && equalChannelAndCN;
}

END_CS_NAMESPACE