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
 * @tparam  NUMBER_OF_BUTTONS
 *          The number of buttons in the collection.
 *
 * @ingroup BankableMIDIOutputElements
 */
template <uint8_t NUMBER_OF_BUTTONS>
class CCButtons : public MIDIButtons<DigitalCCSender, NUMBER_OF_BUTTONS> {
  public:
    /**
     * @brief   Create a new Bankable CCButtons object with the given pins,
     *          the given controller number and channel.
     *
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   buttons
     *          An list of digital input pins with the buttons connected.  
     *          The internal pull-up resistors will be enabled.
     * @param   baseAddress
     *          The MIDI address of the first button, containing the controller
     *          number [0, 119], channel [CHANNEL_1, CHANNEL_16], and optional 
     *          cable number [0, 15].
     * @param   incrementAddress
     *          The number of addresses to increment for each next button.  
     *          E.g. if `baseAddress` is 8, and `incrementAddress` is 2,
     *          then the first button will send on address 8, the second
     *          button will send on address 10, button three on address 12, etc.
     * @param   sender
     *          The MIDI sender to use.
     */
    CCButtons(const OutputBankConfig &config,
              const Array<AH::Button, NUMBER_OF_BUTTONS> &buttons,
              const MIDICNChannelAddress &baseAddress,
              const RelativeMIDICNChannelAddress &incrementAddress,
              const DigitalCCSender &sender = {})
        : MIDIButtons<DigitalCCSender, NUMBER_OF_BUTTONS>(
              config, buttons, baseAddress, incrementAddress, sender) {}
};

} // namespace Bankable

END_CS_NAMESPACE