#pragma once

#include <Def/MIDIAddress.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   An abstract class for output elements attached to an address.
 *
 * @see     MIDIAddress
 */
class MIDIAddressable
{
public:
    /**
     * @brief   Construct a new MIDIButton.
     *
     * @param   address
     *          The MIDI address to send to.
     */
    MIDIAddressable(MIDIAddress address): address(address) {};

    /// Get the MIDI address.
    MIDIAddress getAddress() const { return this->address; }
    /// Set the MIDI address.
    void setAddress(MIDIAddress address) { this->address = address; }

protected:
    MIDIAddress address;
};

END_CS_NAMESPACE