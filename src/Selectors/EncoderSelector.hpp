#pragma once

#include "Selector.hpp"
#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <Def/Def.hpp>
#include <Def/TypeTraits.hpp>

#ifdef ARDUINO
#include <Submodules/Encoder/Encoder.h>
#else
#include <Encoder.h> // Mock
#endif

BEGIN_CS_NAMESPACE

template <setting_t N, class Callback = EmptySelectorCallback>
class GenericEncoderSelector : public GenericSelector<N, Callback> {
    using Parent = GenericSelector<N, Callback>;

  public:
    GenericEncoderSelector(Selectable<N> &selectable, const Callback &callback,
                           const EncoderSwitchPinList &pins,
                           int8_t pulsesPerStep = 4, Wrap wrap = Wrap::Wrap)
        : GenericSelector<N, Callback> {selectable, callback}, encoder {pins.A,
                                                                        pins.B},
          switchPin(pins.switchPin), pulsesPerStep(pulsesPerStep), wrap(wrap) {}

    void begin() override {
        Parent::begin();
        if (switchPin != NO_PIN)
            AH::ExtIO::pinMode(switchPin, INPUT_PULLUP);
        begin_if_possible(encoder);
    }

    void update() override {
        Parent::update();
        // TODO: use EncoderState
        long currentPosition = encoder.read();
        long difference = (currentPosition - previousPosition) / pulsesPerStep;
        if (difference) {
            previousPosition += difference * pulsesPerStep;
            if (difference > 0)
                while (difference-- > 0)
                    this->increment(wrap);
            else
                while (difference++ < 0)
                    this->decrement(wrap);
        }

        if (switchPin != NO_PIN) {
            bool currentState = AH::ExtIO::digitalRead(switchPin);
            if (previousSwitchState == HIGH && currentState == LOW) {
                // TODO: invert?
                this->reset();
            }
            previousSwitchState = currentState;
        }
    }

  private:
    Encoder encoder;
    pin_t switchPin;
    int8_t pulsesPerStep;
    Wrap wrap;

    long previousPosition = 0;
    bool previousSwitchState = HIGH;
};

// -------------------------------------------------------------------------- //

/**
 * @brief   Selector that reads from a rotary encoder. 
 * 
 * @ingroup Selectors
 * 
 * @tparam  N 
 *          The number of settings.
 */
template <setting_t N>
class EncoderSelector : public GenericEncoderSelector<N> {
  public:
    EncoderSelector(Selectable<N> &selectable, const EncoderSwitchPinList &pins,
                    int8_t pulsesPerStep = 4, Wrap wrap = Wrap::Wrap)
        : GenericEncoderSelector<N> {
              selectable, {}, pins, pulsesPerStep, wrap,
          } {}
};

END_CS_NAMESPACE