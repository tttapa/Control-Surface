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
        set(0);
        beginInput();
    }

    virtual void reset() { set(0); }

    setting_t get() const { return setting; }

    void set(setting_t newSetting) {
        newSetting = validateSetting(newSetting);
        selectable.select(newSetting);
        updateOutput(get(), newSetting);
        setting = newSetting;
    }

    constexpr setting_t getNumberOfSettings() const { return N; }

  private:
    virtual void updateOutput(setting_t oldSetting, setting_t newSetting) = 0;

    setting_t validateSetting(setting_t setting) const {
        static_assert(setting_t(-1) > setting_t(0),
                      "Error: setting_t should be an unsigned integer type.");
        if (setting >= getNumberOfSettings()) {
            DEBUGFN(F("Error: Setting ")
                    << setting
                    << F(" is not less than the total number of settings (")
                    << getNumberOfSettings() << ')');
            ERROR(return getNumberOfSettings() - 1);
        }
        return setting;
    }

    Selectable<N> &selectable;
    setting_t setting = 0;
};