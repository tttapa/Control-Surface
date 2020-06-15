#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIButton.hpp>
#include <MIDI_Senders/ProgramChangeSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **momentary
 *          push button or switch**, and send out MIDI **Program Change** 
 *          events.
 * 
 * A Program Change event is sent when the button is pressed.  
 * The button is debounced in software.  
 * This version can be banked.
 *
 * @ingroup BankableMIDIOutputElements
 */
class PCButton : public MIDIButton<SingleAddress, ProgramChangeSender> {
  public:
    /**
     * @brief   Create a new PCButton object with the given pin, program number
     *          and channel.
     * 
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   pin
     *          The digital input pin to read from.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address containing the program number [0, 127], 
     *          channel [CHANNEL_1, CHANNEL_16], and optional cable number 
     *          [CABLE_1, CABLE_16].
     */
    PCButton(OutputBankConfig<> config, pin_t pin, MIDIAddress address)
        : MIDIButton({config, address}, pin, {}) {}
};

} // namespace Bankable

END_CS_NAMESPACE