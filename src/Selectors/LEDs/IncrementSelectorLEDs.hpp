#pragma once

#include "SelectorLEDs.hpp"
#include <Selectors/IncrementSelector.hpp>

template <setting_t NUMBER_OF_LEDs>
class IncrementSelectorLEDs : public IncrementSelector_Base,
                              public SelectorLEDs<NUMBER_OF_LEDs> {
  public:
    IncrementSelectorLEDs(Selectable &selectable, const Button &button,
                          const PinList<NUMBER_OF_LEDs> &ledPins,
                          bool wrap = true)
        : Selector(selectable, NUMBER_OF_LEDs),
          IncrementSelector_Base(button, wrap), SelectorLEDs<NUMBER_OF_LEDs>(
                                                    ledPins) {}
};