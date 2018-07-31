#pragma once

#include <Control_Surface/Control_Surface_Class.h>
#include <Def/Def.hpp>
#include <Selectors/Selectable.hpp>

template <setting_t N>
class ProgramChanger : public Selectable<N> {
  public:
    ProgramChanger(const Array<uint8_t, N> &programs, uint8_t channel)
        : programs(programs), channel(channel) {}

    void select(setting_t setting) override {
        setting = validateSetting(setting);
        Control_Surface.MIDI().send(PROGRAM_CHANGE, channel, programs[setting]);
    }

  private:
    setting_t validateSetting(setting_t setting) const {
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

    const Array<uint8_t, N> programs;
    const uint8_t channel;
};