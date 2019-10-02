#pragma once

#include "Selector.hpp"
#include <Def/Def.hpp>
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N>
class ManyButtonsSelector_Base : virtual public Selector<N> {
  protected:
    ManyButtonsSelector_Base(const PinList<N> &buttonPins)
        : buttonPins(buttonPins) {}

  public:
    void beginInput() override {
        for (const pin_t &pin : buttonPins)
            ExtIO::pinMode(pin, INPUT_PULLUP);
    }

    void update() override {
        for (setting_t i = 0; i < N; i++)
            // TODO: invert?
            if (ExtIO::digitalRead(buttonPins[i]) == LOW) {
                if (this->get() != i)
                    this->set(i);
                break;
            }
    }

  private:
    const PinList<N> buttonPins;
};

// -------------------------------------------------------------------------- //

/**
 * @brief   Selector that reads from \(N\) buttons.
 * 
 * Pressing the \(n\)-th button selects the \(n\)-th setting.
 * 
 * @ingroup Selectors
 * 
 * @tparam  N 
 *          The number of settings.
 */
template <setting_t N>
class ManyButtonsSelector : public ManyButtonsSelector_Base<N> {
  public:
    ManyButtonsSelector(Selectable<N> &selectable, const PinList<N> &buttonPins)
        : Selector<N>(selectable), ManyButtonsSelector_Base<N>(buttonPins) {}

    void beginOutput() override {}
    void updateOutput(UNUSED_PARAM setting_t oldSetting,
                      UNUSED_PARAM setting_t newSetting) override {}
};

END_CS_NAMESPACE