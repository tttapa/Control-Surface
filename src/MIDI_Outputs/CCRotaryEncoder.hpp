#pragma once

#include <MIDI_Outputs/Abstract/MIDIRotaryEncoder.hpp>
#include <MIDI_Senders/RelativeCCSender.hpp>

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **quadrature
 *          (rotary) encoder** and send out relative MIDI **Control Change**
 *          events.
 * 
 *          This version cannot be banked.
 *
 * @note    To use this class, include the [PJRC Encoder library]
 *          (https://github.com/PaulStoffregen/Encoder) before the
 *          Control-Surface library.
 *
 * @ingroup MIDIOutputElements
 */
class CCRotaryEncoder : public MIDIRotaryEncoder<RelativeCCSender::send> {
  public:
    /**
     * @brief   Construct a new CCRotaryEncoder object with the given pins, 
     *          address, channel, speed factor, and number of pulses per step.
     * 
     * @param   pins
     *          A list of the two pins connected to the A and B outputs of the
     *          encoder.  
     *          The internal pull-up resistors will be enabled by the Encoder
     *          library.
     * @param   address
     *          The MIDI controller number. [0, 119]
     * @param   channel
     *          The MIDI channel. [1, 16]
     * @param   speedMultiply
     *          A constant factor to increase the speed of the rotary encoder.
     *          The difference in position will just be multiplied by this 
     *          factor. 
     * @param   pulsesPerStep
     *          The number of pulses per physical click of the encoder.
     *          For a normal encoder, this is 4. If you want to increase the
     *          resolution, for the use of Jog wheels, for example, you can go
     *          as 1.  
     *          Whereas a greater speedMultiply factor will increase the speed,
     *          increasing the number of pulsesPerStep will result in a slower
     *          speed.
     * 
     * @ingroup MIDIOutputElementConstructors
     */
    CCRotaryEncoder(const EncoderPinList &pins, uint8_t address,
                    uint8_t channel = 1, uint8_t speedMultiply = 1,
                    uint8_t pulsesPerStep = 4)
        : MIDIRotaryEncoder(pins, address, channel,
                                              speedMultiply, pulsesPerStep) {}

// For tests only
#ifndef ARDUINO
    CCRotaryEncoder(const Encoder &encoder, uint8_t address, uint8_t channel,
                    uint8_t speedMultiply, uint8_t pulsesPerStep)
        : MIDIRotaryEncoder(encoder, address, channel,
                                              speedMultiply, pulsesPerStep) {}
#endif
};