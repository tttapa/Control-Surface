#pragma once

#include "SelectorLEDs.hpp"
#include <Selectors/IncrementDecrementSelector.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N>
class IncrementDecrementSelectorLEDs
    : public IncrementDecrementSelector_Base<N>,
      public SelectorLEDs<N> {
  public:
    IncrementDecrementSelectorLEDs(Selectable<N> &selectable,
                                   const IncrementDecrementButtons &buttons,
                                   const PinList<N> &ledPins,
                                   Wrap wrap = Wrap::Wrap)
        : Selector<N>(selectable),
          IncrementDecrementSelector_Base<N>(buttons, wrap),
          SelectorLEDs<N>(ledPins) {}
};

END_CS_NAMESPACE