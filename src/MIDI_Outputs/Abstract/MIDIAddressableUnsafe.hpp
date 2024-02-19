#pragma once

#include <Def/MIDIAddress.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   An abstract class for output elements attached to an address.
 *
 * @see     MIDIAddress
 */
class MIDIAddressableUnsafe
{
public:
    /**
     * @brief   Construct a new MIDIButton.
     *
     * @param   address
     *          The MIDI address to send to.
     */
    MIDIAddressableUnsafe(MIDIAddress address): address(address) {};

    /// Get the MIDI address.
    MIDIAddress getAddress() const { return this->address; }
    /// Set the MIDI address. Has unexpected consequences if used while a
    /// push button is pressed. Use banks if you need to support that.
    void setAddressUnsafe(MIDIAddress address) { this->address = address; }

protected:
    MIDIAddress address;
};

END_CS_NAMESPACE