#pragma once

#include <Helpers/Debug.hpp>
#include <stdint.h>

using setting_t = uint8_t;

template <setting_t N> // TODO: check bounds here?
class Selectable {
  public:
    Selectable(setting_t initialSelection = 0)
        : initialSelection(initialSelection) {}
    virtual void select(setting_t setting) = 0;
    static setting_t validateSetting(setting_t setting) {
        static_assert(setting_t(-1) > setting_t(0),
                      "Error: setting_t should be an unsigned integer type.");
        if (setting >= N) {
            DEBUGFN(F("Error: Setting ")
                    << setting
                    << F(" is not less than the number of programs (") << N
                    << ')');
            ERROR(return N - 1);
        }
        return setting;
    }
    setting_t getInitialSelection() { return initialSelection; }

  private:
    const setting_t initialSelection;
};