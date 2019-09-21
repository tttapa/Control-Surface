#pragma once

#include "SelectorLEDs.hpp"
#include <Selectors/EncoderSelector.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N>
class EncoderSelectorLEDs : public EncoderSelector_Base<N>,
                            public SelectorLEDs<N> {
  public:
    EncoderSelectorLEDs(Selectable<N> &selectable, const PinList<N> &ledPins,
                        const EncoderSwitchPinList &pins,
                        uint8_t pulsesPerStep = 4, Wrap wrap = Wrap::Wrap)
        : Selector<N>(selectable), EncoderSelector_Base<N>(pins, pulsesPerStep,
                                                           wrap),
          SelectorLEDs<N>(ledPins) {}
};

END_CS_NAMESPACE