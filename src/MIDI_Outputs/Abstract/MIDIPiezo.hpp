#pragma once

#include <AH/Hardware/Piezo.hpp>
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   An abstract class for analog buttons or sensors
 *  (i.e. a piezo) that send MIDI events.
 *
 * The piezo is debounced.
 *
 * @see     Piezo
 */
template <class Sender>
class MIDIPiezo : public MIDIOutputElement {
  public:
    /**
     * @brief   Construct a new MIDIPiezo.
     *
     * @param   pin
     *          The digital input pin with the piezo connected.
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address to send to.
     * @param   sender
     *          The MIDI sender to use.
     */
    MIDIPiezo(pin_t pin, MIDIAddress address, const Sender &sender)
        : piezo(pin), address(address), sender(sender) {}

    void begin() override { piezo.begin(); }
    void update() override {
        AH::Piezo::State state = piezo.update();
        if (state == AH::Piezo::Struck) {
            sender.sendOn(address, piezo.getVelocity());
            // piezo.dropState();
        } else if (state == AH::Piezo::TurnOff) {
            sender.sendOff(address);
        } //else if (state == AH::Piezo::Idle) {
        //}
    }

    AH::Piezo::State getButtonState() const { return piezo.getState(); }

    // Sets the minimum threshold for a hit to be detected.
    // If this is too low, you will get random hits. Too high
    // and it will not detect hits. 
    void setHitThreshold(uint16_t hitThreshold) {
        piezo.setHitThreshold(hitThreshold);
    }

    // samples the background noise and returns the highest
    // level detected (0-1023)
    uint16_t sampleSilence(uint16_t sampleTime) {
        return piezo.sampleSilence(sampleTime);
    }
    
  private:
    AH::Piezo piezo;
    const MIDIAddress address;

  public:
    Sender sender;
};

END_CS_NAMESPACE