#include "MIDIAddress.hpp"

BEGIN_CS_NAMESPACE

RelativeMIDIAddress &RelativeMIDIAddress::operator+=(RelativeMIDIAddress that) {
    this->addresses.valid &= that.addresses.valid;
    this->addresses.address += that.addresses.address;
    this->addresses.channel += that.addresses.channel;
    this->addresses.cableNumber += that.addresses.cableNumber;
    return *this;
}

MIDIAddress &MIDIAddress::operator+=(RelativeMIDIAddress rhs) {
    this->addresses.valid &= rhs.addresses.valid;
    this->addresses.address += rhs.addresses.address;
    this->addresses.channel += rhs.addresses.channel;
    this->addresses.cableNumber += rhs.addresses.cableNumber;
    return *this;
}

MIDIAddress &MIDIAddress::operator-=(RelativeMIDIAddress rhs) {
    this->addresses.valid &= rhs.addresses.valid;
    this->addresses.address -= rhs.addresses.address;
    this->addresses.channel -= rhs.addresses.channel;
    this->addresses.cableNumber -= rhs.addresses.cableNumber;
    return *this;
}

MIDIAddress MIDIAddress::operator+(RelativeMIDIAddress rhs) const {
    MIDIAddress copy = *this;
    copy += rhs;
    return copy;
}

MIDIAddress MIDIAddress::operator-(RelativeMIDIAddress rhs) const {
    MIDIAddress copy = *this;
    copy -= rhs;
    return copy;
}

bool MIDIAddress::matchAddressInRange(MIDIAddress toMatch, MIDIAddress base,
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