#pragma once

#include "SelectorLEDs.hpp"
#include <Selectors/IncrementDecrementSelector.hpp>

template <setting_t NUMBER_OF_LEDs>
class IncrementDecrementSelectorLEDs : public IncrementDecrementSelector_Base,
                                       public SelectorLEDs<NUMBER_OF_LEDs> {
  public:
    IncrementDecrementSelectorLEDs(Selectable &selectable,
                                   const IncrementDecrementButtons &buttons,
                                   const PinList<NUMBER_OF_LEDs> &ledPins,
                                   bool wrap = true)
        : Selector(selectable, NUMBER_OF_LEDs),
          IncrementDecrementSelector_Base(buttons, wrap),
          SelectorLEDs<NUMBER_OF_LEDs>(ledPins) {}
};