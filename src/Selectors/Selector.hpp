#pragma once

#include "Selectable.hpp"
#include <Def/Def.hpp>
#include <Helpers/Debug.hpp>
#include <Helpers/Updatable.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   An enumeration to set the behavior of selectors that are incremented
 *          (decremented) beyond their maximum (minimum) setting.
 */
enum class Wrap : bool {
    Clamp = false, ///< When the maximum (minimum) setting is reached,
                   ///< clamp to the maximum (minimum) setting.
                   ///< @todo  Rename to `Clamp`?
    Wrap = true,   ///< When the maximum (minimum) setting is reached,
                   ///< wrap around to the minimum (maximum) setting.
    NoWrap = false,
};

struct EmptySelectorCallback {
    /// Initialize.
    void begin() {}
    /// Refresh, called periodically.
    void update() {}
    /// Called when the setting changes.
    void update(setting_t oldSetting, setting_t newSetting) {
        (void)oldSetting, (void)newSetting;
    }
};

template <setting_t N, class Callback = EmptySelectorCallback>
class GenericSelector : public Updatable<> {
  protected:
    GenericSelector(Selectable<N> &selectable, const Callback &callback)
        : selectable{selectable}, callback{callback} {}

  public:
    void begin() override {
        callback.begin();
        reset();
    }

    void update() override { callback.update(); }

    void reset() { set(selectable.getInitialSelection()); }

    setting_t get() const { return setting; }

    void set(setting_t newSetting) {
        newSetting = Selectable<N>::validateSetting(newSetting);
        selectable.select(newSetting);
        if (get() != newSetting) {
            callback.update(get(), newSetting);
            setting = newSetting;
        }
    }

    void increment(Wrap wrap) {
        setting_t setting = this->get();
        setting++;
        if (setting == N) {
            if (wrap == Wrap::Wrap)
                setting = 0;
            else
                return;
        }
        this->set(setting);
    }

    void decrement(Wrap wrap) {
        setting_t setting = this->get();
        if (setting == 0) {
            if (wrap == Wrap::Wrap)
                setting = N;
            else
                return;
        }
        setting--;
        this->set(setting);
    }

  private:
    Selectable<N> &selectable;
    Callback callback;
    setting_t setting = 0;
};

template <setting_t N>
class Selector : public GenericSelector<N> {
  public:
    Selector(Selectable<N> &selectable) : GenericSelector<N>{selectable, {}} {}
};

END_CS_NAMESPACE