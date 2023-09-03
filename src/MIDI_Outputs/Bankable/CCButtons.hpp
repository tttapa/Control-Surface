#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIButtons.hpp>
#include <MIDI_Senders/DigitalCCSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of **a collection
 *          of momentary push buttons or switches**, and send out MIDI **Control
 *          Change** events.
 * 
 * A value of 0x7F is sent when a button is pressed, and a value of 0x00 is sent
 * when a button is released.  
 * The buttons are debounced in software.  
 * This version can be banked.  
 *
 * @tparam  NumButtons
 *          The number of buttons in the collection.
 *
 * @ingroup BankableMIDIOutputElements
 */
template <uint8_t NumButtons>
class CCButtons
    : public MIDIButtons<SingleAddress, DigitalCCSender, NumButtons> {
  public:
    /**
     * @brief   Create a new Bankable CCButtons object with the given pins,
     *          the given controller number and channel.
     *
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   buttons
     *          A list of digital input pins with the buttons connected.  
     *          The internal pull-up resistors will be enabled.
     * @param   baseAddress
     *          The MIDI address of the first button, containing the controller
     *          number [0, 119], channel [Channel_1, Channel_16], and optional 
     *          cable number [Cable_1, Cable_16].
     * @param   incrementAddress
     *          The number of addresses to increment for each next button.  
     *          E.g. if `baseAddress` is 8, and `incrementAddress` is 2,
     *          then the first button will send on address 8, the second
     *          button will send on address 10, button three on address 12, etc.
     * @param   sender
     *          The MIDI sender to use.
     */
    CCButtons(OutputBankConfig<> config,
              const Array<AH::Button, NumButtons> &buttons,
              MIDIAddress baseAddress, RelativeMIDIAddress incrementAddress,
              const DigitalCCSender &sender = {})
        : MIDIButtons<SingleAddress, DigitalCCSender, NumButtons>(
              {config, baseAddress}, buttons, incrementAddress, sender) {}
};

} // namespace Bankable

END_CS_NAMESPACE