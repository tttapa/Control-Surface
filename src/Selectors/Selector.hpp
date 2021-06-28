#pragma once

#include "Selectable.hpp"
#include <AH/Containers/Updatable.hpp>
#include <AH/Debug/Debug.hpp>
#include <Def/Def.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   An enumeration to set the behavior of selectors that are incremented
 *          (decremented) beyond their maximum (minimum) setting.
 */
enum class Wrap : bool {
    Clamp = false, ///< When the maximum (minimum) setting is reached,
                   ///< clamp to the maximum (minimum) setting.
    Wrap = true,   ///< When the maximum (minimum) setting is reached,
                   ///< wrap around to the minimum (maximum) setting.
    NoWrap = false,
};

/// A callback for the GenericSelector class that does nothing.
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

/// Base class for all Selectors exposing the `get` method, so it can be used
/// by display elements etc, without having to provide the full generic type.
///
/// A `set` method is not provided, because that would require either more
/// virtual functions, or a rather large refactoring.
class SelectorBase {
  protected:
    /// Constructor.
    SelectorBase() = default;

  public:
    /// Get the current selection/setting.
    setting_t get() const { return setting; }

  protected:
    /// The selection of the selector. It is saved in the selector as well as
    /// the selectable, because you need it in order to implement
    /// increment/decrement methods.
    setting_t setting = 0;
};

template <setting_t N, class Callback = EmptySelectorCallback>
class GenericSelector : public SelectorBase, public AH::Updatable<> {
  public:
    /**
     * @brief   Constructor.
     * 
     * @param   selectable
     *          The selectable object to manage. When the value of the selector
     *          changes, it changes the selection of this selectable.
     * @param   callback
     *          The callback to call when the value changes. Used for (visual)
     *          feedback from the selector (e.g. LEDs or some different kind of
     *          display).
     */
    GenericSelector(Selectable<N> &selectable, const Callback &callback)
        : selectable(selectable), callback(callback) {}

    void begin() override {
        callback.begin();
        reset();
    }

    void update() override { callback.update(); }

    /// Reset the selection to the initial selection.
    void reset() {
        setting_t initialSelection = selectable.getInitialSelection();
        selectable.select(initialSelection);
        callback.update(initialSelection, initialSelection);
        this->setting = initialSelection;
    }

    /** 
     * @brief   Select the given selection
     * 
     * @param   newSetting
     *          The new setting to select [0, N-1].
     */
    void set(setting_t newSetting) {
        newSetting = selectable.validateSetting(newSetting);
        selectable.select(newSetting);
        if (get() != newSetting) {
            callback.update(get(), newSetting);
            this->setting = newSetting;
        }
    }

    /**
     * @brief   Add one to the setting, wrap around or clamp, depending on the
     *          parameter, if the new setting would be out of range.
     * 
     * @param   wrap 
     *          Wrap or clamp if the new setting would be out of range.
     */
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

    /**
     * @brief   Subtract one from the setting, wrap around or clamp, depending 
     *          on the parameter, if the new setting would be out of range.
     * 
     * @param   wrap 
     *          Wrap or clamp if the new setting would be out of range.
     */
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

  public:
    Callback callback;
};

/** 
 * @brief   A Selector with an empty callback.
 * 
 * @tparam  N
 *          The number of possible settings.
 */
template <setting_t N>
class Selector : public GenericSelector<N> {
  public:
    /// Constructor
    Selector(Selectable<N> &selectable) : GenericSelector<N>{selectable, {}} {}
};

END_CS_NAMESPACE