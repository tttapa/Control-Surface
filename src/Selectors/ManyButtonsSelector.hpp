#pragma once

#include "Selector.hpp"
#include <Def/Def.hpp>
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N, class Callback = EmptySelectorCallback>
class GenericManyButtonsSelector : public GenericSelector<N, Callback> {
  protected:
    GenericManyButtonsSelector(Selectable<N> &selectable,
                               const Callback &callback,
                               const PinList<N> &buttonPins)
        : GenericSelector<N, Callback>{selectable, callback}, buttonPins{
                                                                  buttonPins} {}

  private:
    using Parent = GenericSelector<N, Callback>;

  public:
    void begin() override {
        Parent::begin();
        for (const pin_t &pin : buttonPins)
            ExtIO::pinMode(pin, INPUT_PULLUP);
    }

    void update() override {
        Parent::update();
        for (setting_t i = 0; i < N; i++)
            // TODO: invert?
            if (ExtIO::digitalRead(buttonPins[i]) == LOW) {
                if (ExtIO::digitalRead(buttonPins[this->get()]) != LOW)
                    this->set(i);
                break;
            }
    }

  private:
    PinList<N> buttonPins;
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
class ManyButtonsSelector : public GenericManyButtonsSelector<N> {
  public:
    ManyButtonsSelector(Selectable<N> &selectable, const PinList<N> &buttonPins)
        : GenericManyButtonsSelector<N>{
              selectable,
              {},
              buttonPins,
          } {}
};

END_CS_NAMESPACE