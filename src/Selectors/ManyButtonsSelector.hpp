#pragma once

#include "Selector.hpp"
#include <AH/Containers/ArrayHelpers.hpp>
#include <AH/Hardware/Button.hpp>
#include <Def/Def.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N, class Callback = EmptySelectorCallback>
class GenericManyButtonsSelector : public GenericSelector<N, Callback> {
    using Parent = GenericSelector<N, Callback>;

  public:
    GenericManyButtonsSelector(Selectable<N> &selectable,
                               const Callback &callback,
                               const PinList<N> &buttonPins)
        : GenericSelector<N, Callback> {selectable, callback},
          buttons(AH::copyAs<AH::Button>(buttonPins)) {}

    void begin() override {
        Parent::begin();
        for (auto &btn : buttons)
            btn.begin();
    }

    void update() override {
        Parent::update();
        for (setting_t i = 0; i < N; i++)
            if (buttons[i].update() == AH::Button::Falling &&
                buttons[this->get()].getState() != AH::Button::Pressed)
                this->set(i);
    }

    void invert() {
        for (auto &btn : buttons)
            btn.invert();
    }

  private:
    AH::Array<AH::Button, N> buttons;
};

// -------------------------------------------------------------------------- //

/**
 * @brief   Selector that reads from @f$ N @f$ buttons.
 * 
 * Pressing the @f$ n @f$-th button selects the @f$ n @f$-th setting.
 * 
 * @htmlonly 
 * <object type="image/svg+xml" data="../../selector-multiple-momentary-switches-LED.svg"></object>
 * @endhtmlonly
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
        : GenericManyButtonsSelector<N> {
              selectable,
              {},
              buttonPins,
          } {}
};

END_CS_NAMESPACE