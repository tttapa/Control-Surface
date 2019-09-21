#pragma once

#include "SelectorLEDs.hpp"
#include <Selectors/IncrementSelector.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N>
class IncrementSelectorLEDs : public IncrementSelector_Base<N>,
                              public SelectorLEDs<N> {
  public:
    IncrementSelectorLEDs(Selectable<N> &selectable, const Button &button,
                          const PinList<N> &ledPins, Wrap wrap = Wrap::Wrap)
        : Selector<N>(selectable),
          IncrementSelector_Base<N>(button, wrap), SelectorLEDs<N>(ledPins) {}
};

END_CS_NAMESPACE