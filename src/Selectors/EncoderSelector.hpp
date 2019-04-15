#pragma once

#if not defined(Encoder_h_) && not defined(IDE)
#error                                                                         \
    "The PJRC Encoder library should be included before the Control-Surface    \
     library. (#include <Encoder.h>)"
#endif

#include "Selector.hpp"
#include <Def/Def.hpp>
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>

template <setting_t N>
class EncoderSelector_Base : virtual public Selector<N> {
  public:
    EncoderSelector_Base(const EncoderSwitchPinList &pins,
                         int8_t pulsesPerStep = 4, Wrap wrap = Wrap::Wrap)
        : encoder{pins.A, pins.B}, switchPin(pins.switchPin),
          pulsesPerStep(pulsesPerStep), wrap(wrap) {}

    void beginInput() override {
        if (switchPin != NO_PIN)
            ExtIO::pinMode(switchPin, INPUT_PULLUP);
    }

    void update() override {
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
            bool currentState = ExtIO::digitalRead(switchPin);
            if (previousSwitchState == HIGH && currentState == LOW) {
                // TODO: invert?
                this->reset();
            }
            previousSwitchState = currentState;
        }
    }

  private:
    Encoder encoder;
    const pin_t switchPin;
    const int8_t pulsesPerStep;
    Wrap wrap;

    long previousPosition = 0;
    bool previousSwitchState = HIGH;
};

// -------------------------------------------------------------------------- //

template <setting_t N>
class EncoderSelector : public EncoderSelector_Base<N> {
  public:
    EncoderSelector(Selectable<N> &selectable, const EncoderSwitchPinList &pins,
                    int8_t pulsesPerStep = 4, Wrap wrap = Wrap::Wrap)
        : Selector<N>(selectable), EncoderSelector_Base<N>(pins, pulsesPerStep,
                                                           wrap) {}

    void beginOutput() override {}
    void updateOutput(setting_t oldSetting, setting_t newSetting) override {}
};