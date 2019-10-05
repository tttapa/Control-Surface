#pragma once

#include "Selector.hpp"
#include <Hardware/IncrementDecrementButtons.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N, class Callback = EmptySelectorCallback>
class GenericIncrementDecrementSelector : public GenericSelector<N, Callback> {
  protected:
    GenericIncrementDecrementSelector(Selectable<N> &selectable,
                                      const Callback &callback,
                                      const IncrementDecrementButtons &buttons,
                                      Wrap wrap = Wrap::Wrap)
        : GenericSelector<N>{selectable, callback}, buttons{buttons},
          wrap{wrap} {}

  private:
    using Parent = GenericSelector<N, Callback>;

  public:
    void begin() override {
        Parent::begin();
        buttons.begin();
    }

    void update() override {
        Parent::update();
        using IncrDecrButtons = IncrementDecrementButtons;
        switch (buttons.getState()) {
            case IncrDecrButtons::Increment: this->increment(wrap); break;
            case IncrDecrButtons::Decrement: this->decrement(wrap); break;
            case IncrDecrButtons::Reset: this->reset(); break;
            case IncrDecrButtons::Nothing: break;
            default: break;
        }
    }

#ifdef INDIVIDUAL_BUTTON_INVERT
    void invert() { buttons.invert(); }
#endif

  private:
    IncrementDecrementButtons buttons;
    Wrap wrap;
};

// -------------------------------------------------------------------------- //

/**
 * @brief   Selector with two buttons (one to increment, one to decrement).
 * 
 * Pressing two buttons simultaneously resets to the default setting.
 * 
 * @ingroup Selectors 
 * 
 * @tparam  N 
 *          The number of settings.
 */
template <setting_t N>
class IncrementDecrementSelector : public GenericIncrementDecrementSelector<N> {
  public:
    IncrementDecrementSelector(Selectable<N> &selectable,
                               const IncrementDecrementButtons &buttons,
                               Wrap wrap = Wrap::Wrap)
        : GenericIncrementDecrementSelector<N>{
              selectable,
              {},
              buttons,
              wrap,
          } {}
};

END_CS_NAMESPACE