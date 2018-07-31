#pragma once

#include "SelectorLEDs.hpp"
#include <Selectors/ManyButtonsSelector.hpp>

template <setting_t N>
class ManyButtonsSelectorLEDs : public ManyButtonsSelector_Base<N>,
                                public SelectorLEDs<N> {
  public:
    ManyButtonsSelectorLEDs(Selectable<N> &selectable,
                            const PinList<N> &buttonPins,
                            const PinList<N> &ledPins)
        : Selector<N>(selectable), ManyButtonsSelector_Base<N>(buttonPins),
          SelectorLEDs<N>(ledPins) {}
};