#pragma once

#include "Selectable.hpp"
#include <Helpers/Debug.hpp>
#include <Helpers/LinkedList.h>
#include <Helpers/Updatable.hpp>

template <setting_t N>
class Selector : public Updatable {
  protected:
    Selector(); // Not used, only for virtual inheritance
    Selector(Selectable<N> &selectable) : selectable(selectable) {}

  public:
    virtual void beginInput() = 0;
    virtual void beginOutput() = 0;

    void begin() final override {
        beginOutput();
        set(selectable.getInitialSelection());
        beginInput();
    }

    virtual void reset() { set(selectable.getInitialSelection()); }

    setting_t get() const { return setting; }

    void set(setting_t newSetting) {
        newSetting = Selectable<N>::validateSetting(newSetting);
        selectable.select(newSetting);
        updateOutput(get(), newSetting);
        setting = newSetting;
    }

  private:
    virtual void updateOutput(setting_t oldSetting, setting_t newSetting) = 0;

    Selectable<N> &selectable;
    setting_t setting = 0;
};