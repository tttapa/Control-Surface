#pragma once

#include "Selector.hpp"
#include <Hardware/IncrementDecrementButtons.hpp>

template <setting_t N>
class IncrementDecrementSelector_Base : public virtual Selector<N> {
  public:
    IncrementDecrementSelector_Base(const IncrementDecrementButtons &buttons,
                                    bool wrap = true)
        : buttons(buttons), wrap(wrap) {}

    void beginInput() override { buttons.begin(); }

    void update() override {
        switch (buttons.getState()) {
            case IncrementDecrementButtons::Increment: increment(); break;
            case IncrementDecrementButtons::Decrement: decrement(); break;
            case IncrementDecrementButtons::Reset: this->reset(); break;
            default: break;
        }
    }

    void increment() {
        setting_t setting = this->get();
        setting++;
        if (setting == N) {
            if (wrap) {
                setting = 0;
            } else {
                return;
            }
        }
        this->set(setting);
    }

    void decrement() {
        setting_t setting = this->get();
        if (setting == 0) {
            if (wrap) {
                setting = N;
            } else {
                return;
            }
        }
        setting--;
        this->set(setting);
    }

  private:
    IncrementDecrementButtons buttons;
    bool wrap;
};

// -------------------------------------------------------------------------- //

template <setting_t N>
class IncrementDecrementSelector : public IncrementDecrementSelector_Base<N> {
  public:
    IncrementDecrementSelector(Selectable<N> &selectable,
                               const IncrementDecrementButtons &buttons,
                               bool wrap = true)
        : Selector<N>(selectable), IncrementDecrementSelector_Base<N>(buttons,
                                                                      wrap) {}

    void beginOutput() override {}
    void updateOutput(setting_t oldSetting, setting_t newSetting) override {}
};