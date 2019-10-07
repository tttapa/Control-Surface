#pragma once

#include "Selector.hpp"
#include <Hardware/IncrementButton.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N, class Callback = EmptySelectorCallback>
class GenericIncrementSelector : public GenericSelector<N, Callback> {
    using Parent = GenericSelector<N, Callback>;

  public:
    GenericIncrementSelector(Selectable<N> &selectable,
                             const Callback &callback,
                             const IncrementButton &button,
                             Wrap wrap = Wrap::Wrap)
        : GenericSelector<N, Callback>{selectable, callback}, button{button},
          wrap{wrap} {}

    void begin() override {
        Parent::begin();
        button.begin();
    }

    void update() override {
        Parent::update();
        if (button.getState() == IncrementButton::Increment)
            this->increment(wrap);
    }

#ifdef INDIVIDUAL_BUTTON_INVERT
    void invert() { button.invert(); }
#endif

  private:
    IncrementButton button;
    Wrap wrap;
};

// -------------------------------------------------------------------------- //

/**
 * @brief   Selector with one button that increments the selection.
 * 
 * @ingroup Selectors 
 * 
 * @tparam  N 
 *          The number of settings.
 */
template <setting_t N>
class IncrementSelector : virtual public GenericIncrementSelector<N> {
  public:
    IncrementSelector(Selectable<N> &selectable, const IncrementButton &button,
                      Wrap wrap = Wrap::Wrap)
        : GenericIncrementSelector<N>{
              selectable,
              {},
              button,
              wrap,
          } {}

    IncrementSelector(Selectable<N> &selectable, const Button &button,
                      Wrap wrap = Wrap::Wrap)
        : GenericIncrementSelector<N>{
              selectable,
              {},
              button,
              wrap,
          } {}
};

END_CS_NAMESPACE