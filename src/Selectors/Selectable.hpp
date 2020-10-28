#pragma once

#include <AH/Error/Error.hpp>
#include <AH/STL/type_traits>
#include <Def/Def.hpp>
#include <stdint.h>

BEGIN_CS_NAMESPACE

template <setting_t N> // TODO: check bounds here?
class Selectable {
  protected:
    Selectable(setting_t initialSelection = 0)
        : initialSelection(initialSelection) {}

  public:
    virtual void select(setting_t setting) = 0;

    static setting_t validateSetting(setting_t setting) {
        static_assert(std::is_unsigned<setting_t>::value,
                      "Error: setting_t should be an unsigned integer type.");
        if (setting >= N) {
            ERROR(F("Error: Setting ")
                      << setting
                      << F(" is not less than the number of settings (") << N
                      << ')',
                  0xFFFE);
            return N - 1;
        }
        return setting;
    }

    setting_t getInitialSelection() const { return initialSelection; }

    void setInitialSelection(setting_t initialSelection) {
        this->initialSelection = validateSetting(initialSelection);
    }

  private:
    setting_t initialSelection;
};

END_CS_NAMESPACE