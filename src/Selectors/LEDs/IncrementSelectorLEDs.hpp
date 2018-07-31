#pragma once

#include "SelectorLEDs.hpp"
#include <Selectors/IncrementSelector.hpp>

template <setting_t N>
class IncrementSelectorLEDs : public IncrementSelector_Base<N>,
                              public SelectorLEDs<N> {
  public:
    IncrementSelectorLEDs(Selectable<N> &selectable, const Button &button,
                          const PinList<N> &ledPins, bool wrap = true)
        : Selector<N>(selectable),
          IncrementSelector_Base<N>(button, wrap), SelectorLEDs<N>(ledPins) {}
};