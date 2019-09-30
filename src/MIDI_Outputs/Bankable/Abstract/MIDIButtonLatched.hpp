#pragma once

#include <Banks/BankableMIDIOutput.hpp>
#include <Def/Def.hpp>
#include <Hardware/Button.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class for momentary buttons and switches that send MIDI events.
 *
 * The button is debounced, and the button is latched (press once to enable, 
 * press again to disable) (toggle).
 *
 * @see     Button
 */
template <class BankAddress, class Sender>
class MIDIButtonLatched : public BankAddress, public MIDIOutputElement {
  protected:
    /**
     * @brief   Create a new bankable MIDIButtonLatched object on the given pin
     *          and address.
     * 
     * @param   config
     *          The bank to add this element to.
     * @param   pin
     *          The digital input pin to read from.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address containing the note/controller number [0, 127],
     *          channel [1, 16], and optional cable number [0, 15].
     */
    MIDIButtonLatched(const BankAddress &bankAddress, pin_t pin,
                      const Sender &sender)
        : BankAddress{bankAddress}, button{pin}, sender{sender} {}

  public:
    void begin() final override { button.begin(); }
    void update() final override {
        Button::State state = button.getState();
        if (state == Button::Falling)
            toggleState();
    }

    bool toggleState() {
        setState(!getState());
        return getState();
    }
    bool getState() const { return state; }
    void setState(bool state) {
        this->state = state;
        if (state) {
            this->lock();
            sender.sendOn(this->getActiveAddress());
        } else {
            sender.sendOff(this->getActiveAddress());
            this->unlock();
        }
    }

  private:
    Button button;
    bool state = false;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE