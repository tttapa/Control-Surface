#pragma once

#include <MIDI_Outputs/Abstract/MIDIAbsoluteEncoder.hpp>
#include <MIDI_Senders/ContinuousCCSender.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **quadrature
 *          (rotary) encoder** and send out absolute MIDI **Control Change**
 *          events.
 * 
 * This version cannot be banked.
 *
 * @ingroup MIDIOutputElements
 */
class CCAbsoluteEncoder : public MIDIAbsoluteEncoder<ContinuousCCSender> {
  public:
    /**
     * @brief   Construct a new CCAbsoluteEncoder object with the given pins, 
     *          address, channel, speed factor, and number of pulses per step.
     * 
     * @param   encoder
     *          The Encoder object to use.  
     *          Usually passed as a list of the two pins connected to the 
     *          A and B outputs of the encoder, e.g. `{2, 3}`.  
     *          The internal pull-up resistors will be enabled by the Encoder
     *          library.
     * @param   address
     *          The MIDI address containing the controller number [0, 119], 
     *          channel [Channel_1, Channel_16], and optional cable number 
     *          [Cable_1, Cable_16].
     * @param   multiplier
     *          A constant factor to increase the speed of the rotary encoder.
     *          The position will just be multiplied by this factor. 
     * @param   pulsesPerStep
     *          The number of pulses per physical click of the encoder.
     *          For a normal encoder, this is 4. If you want to increase the
     *          resolution, for the use of Jog wheels, for example, you can go
     *          as 1.  
     *          Whereas a greater speedMultiplier factor will increase the 
     *          speed, increasing the number of pulsesPerStep will result in a 
     *          lower speed.
     */
    CCAbsoluteEncoder(AHEncoder &&encoder, MIDIAddress address,
                      int16_t multiplier = 1, uint8_t pulsesPerStep = 4)
        : MIDIAbsoluteEncoder<ContinuousCCSender>(
              std::move(encoder), address, multiplier, pulsesPerStep, {}) {}
};

END_CS_NAMESPACE