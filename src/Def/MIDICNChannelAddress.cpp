#include "MIDICNChannelAddress.hpp"

BEGIN_CS_NAMESPACE

MIDICNChannelAddress &MIDICNChannelAddress::
operator+=(const RelativeMIDICNChannelAddress &rhs) {
    if (!this->isValid() || !rhs.isValid()) {
        this->addresses.valid = false;
    } else {
        this->addresses.address += rhs.addresses.address;
        this->addresses.channel += rhs.addresses.channel;
        this->addresses.cableNumber += rhs.addresses.cableNumber;
    }
    return *this;
}

MIDICNChannelAddress &MIDICNChannelAddress::
operator-=(const RelativeMIDICNChannelAddress &rhs) {
    if (!this->isValid() || !rhs.isValid()) {
        this->addresses.valid = false;
    } else {
        this->addresses.address -= rhs.addresses.address;
        this->addresses.channel -= rhs.addresses.channel;
        this->addresses.cableNumber -= rhs.addresses.cableNumber;
    }
    return *this;
}

MIDICNChannelAddress MIDICNChannelAddress::
operator+(const RelativeMIDICNChannelAddress &rhs) const {
    MIDICNChannelAddress copy = *this;
    copy += rhs;
    return copy;
}

MIDICNChannelAddress MIDICNChannelAddress::
operator-(const RelativeMIDICNChannelAddress &rhs) const {
    MIDICNChannelAddress copy = *this;
    copy -= rhs;
    return copy;
}

bool MIDICNChannelAddress::matchSingle(const MIDICNChannelAddress &toMatch,
                                       const MIDICNChannelAddress &base) {
    return base == toMatch;
}

bool MIDICNChannelAddress::matchAddressInRange(
    const MIDICNChannelAddress &toMatch, const MIDICNChannelAddress &base,
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