#include <MIDI_Outputs/Bankable/Abstract/MIDIButton.hpp>
#include <MIDI_Senders/DigitalCCSender.hpp>

namespace Bankable {

class CCButton : public MIDIButton<DigitalCCSender> {
  public:
    CCButton(pin_t pin, uint8_t controller, uint8_t channel)
        : MIDIButton<DigitalCCSender>(pin, controller, channel) {}
};

} // namespace Bankable