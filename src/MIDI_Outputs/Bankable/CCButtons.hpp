#include <MIDI_Outputs/Bankable/Abstract/MIDIButtons.hpp>
#include <MIDI_Senders/DigitalCCSender.hpp>

namespace Bankable {

template <uint8_t NUMBER_OF_BUTTONS>
class CCButtons : public MIDIButtons<NUMBER_OF_BUTTONS, DigitalCCSender> {
  public:
    CCButtons(const Array<Button, NUMBER_OF_BUTTONS> &buttons,
              uint8_t baseAddress, uint8_t baseChannel,
              uint8_t addressIncrement, uint8_t channelIncrement)
        : MIDIButtons<NUMBER_OF_BUTTONS, DigitalCCSender>(
              buttons, baseAddress, baseChannel, addressIncrement,
              channelIncrement) {}
};

} // namespace Bankable