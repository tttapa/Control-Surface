#pragma once

#include "SelectorLEDs.hpp"
#include <Selectors/IncrementSelector.hpp>

template <setting_t NUMBER_OF_LEDs>
class IncrementSelectorLEDs : public IncrementSelector,
                              public SelectorLEDs<NUMBER_OF_LEDs> {
  public:
    IncrementSelectorLEDs(Selectable &selectable, const Button &button,
                          const PinList<NUMBER_OF_LEDs> &ledPins,
                          bool wrap = true)
        : Selector(selectable, NUMBER_OF_LEDs),
          IncrementSelector(selectable, NUMBER_OF_LEDs, button, wrap),
          SelectorLEDs<NUMBER_OF_LEDs>(ledPins) {}
};