#pragma once

#include <MIDI_Outputs/Abstract/MIDIAbsoluteEncoder.hpp>
#include <MIDI_Senders/PitchBendSender.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **quadrature
 *          (rotary) encoder** and send out absolute MIDI **Pitch Bend**
 *          events.
 * 
 * This version cannot be banked.
 *
 * @note    To use this class, include the [PJRC Encoder library]
 *          (https://github.com/PaulStoffregen/Encoder) before the
 *          Control-Surface library.
 *
 * @ingroup MIDIOutputElements
 */
class PBAbsoluteEncoder : public MIDIAbsoluteEncoder<PitchBendSender<14>> {
  public:
    /**
     * @brief   Construct a new PBAbsoluteEncoder object with the given pins, 
     *          address, channel, speed factor, and number of pulses per step.
     * 
     * @param   pins
     *          A list of the two pins connected to the A and B outputs of the
     *          encoder.  
     *          The internal pull-up resistors will be enabled by the Encoder
     *          library.
     * @param   address
     *          The MIDI channel [CHANNEL_1, CHANNEL_16] and optional Cable
     *          Number [0, 15].
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
    PBAbsoluteEncoder(const EncoderPinList &pins, const MIDIChannelCN &address,
                      int16_t multiplier = 1, uint8_t pulsesPerStep = 4)
        : MIDIAbsoluteEncoder(pins, address, multiplier, pulsesPerStep,
                              {}) {}
};

END_CS_NAMESPACE