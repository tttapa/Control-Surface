#pragma once

#include "SelectorLEDs.hpp"
#include <Selectors/ManyButtonsSelector.hpp>

template <setting_t NUMBER_OF_LEDs>
class ManyButtonsSelectorLEDs : public ManyButtonsSelector_Base<NUMBER_OF_LEDs>,
                                public SelectorLEDs<NUMBER_OF_LEDs> {
  public:
    ManyButtonsSelectorLEDs(Selectable &selectable,
                            const PinList<NUMBER_OF_LEDs> &buttonPins,
                            const PinList<NUMBER_OF_LEDs> &ledPins)
        : Selector(selectable, NUMBER_OF_LEDs),
          ManyButtonsSelector_Base<NUMBER_OF_LEDs>(buttonPins),
          SelectorLEDs<NUMBER_OF_LEDs>(ledPins) {}
};