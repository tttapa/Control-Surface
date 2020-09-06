#pragma once

#include <Banks/BankAddresses.hpp>
#include <MIDI_Outputs/Bankable/Abstract/MIDIAbsoluteEncoder.hpp>
#include <MIDI_Senders/PitchBendSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **quadrature
 *          (rotary) encoder** and send out absolute MIDI **Control Change**
 *          events.
 * 
 * This version can be banked.
 *
 * @note    To use this class, include the [PJRC Encoder library]
 *          (https://github.com/PaulStoffregen/Encoder) before the
 *          Control-Surface library.
 *
 * @tparam  NumBanks
 *          The number of banks.
 * 
 * @ingroup BankableMIDIOutputElements
 */
template <size_t NumBanks>
class PBAbsoluteEncoder
    : public MIDIAbsoluteEncoder<NumBanks, SingleAddress, PitchBendSender<14>> {
  public:
    /**
     * @brief   Construct a new Bankable PBAbsoluteEncoder object with the given 
     *          pins, controller, channel, speed factor, and number of pulses
     *          per step.
     * 
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   encoder
     *          The Encoder object to use.  
     *          Usually passed as a list of the two pins connected to the 
     *          A and B outputs of the encoder, e.g. `{2, 3}`.  
     *          The internal pull-up resistors will be enabled by the Encoder
     *          library.
     * @param   address
     *          The MIDI address containing the controller number [0, 119], 
     *          channel [CHANNEL_1, CHANNEL_16], and optional cable number 
     *          [0, 15].
     * @param   speedMultiply
     *          A constant factor to increase the speed of the rotary encoder.
     *          The difference in position will just be multiplied by this 
     *          factor. 
     * @param   pulsesPerStep
     *          The number of pulses per physical click of the encoder.
     *          For a normal encoder, this is 4. If you want to increase the
     *          resolution, for the use of Jog wheels, for example, you can go
     *          as 1.  
     *          Whereas a greater speedMultiply factor will increase the 
     *          speed, increasing the number of pulsesPerStep will result in a 
     *          lower speed.
     */
    PBAbsoluteEncoder(const BankConfig<NumBanks> &config, Encoder &&encoder,
                      const MIDIAddress &address, int16_t speedMultiply = 1,
                      uint8_t pulsesPerStep = 4)
        : MIDIAbsoluteEncoder<NumBanks, SingleAddress, PitchBendSender<14>>(
              {config, address}, std::move(encoder), speedMultiply,
              pulsesPerStep, {}) {}
};

} // namespace Bankable

END_CS_NAMESPACE