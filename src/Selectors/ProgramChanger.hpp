#pragma once

#include <Control_Surface/Control_Surface_Class.hpp>
#include <Def/Def.hpp>
#include <Selectors/Selectable.hpp>

template <setting_t N>
class ProgramChanger : public Selectable<N> {
  public:
    ProgramChanger(const Array<uint8_t, N> &programs, uint8_t channel)
        : programs(programs), channel(channel) {}

    void select(setting_t setting) override {
        setting = this->validateSetting(setting);
        Control_Surface.MIDI().send(PROGRAM_CHANGE, channel, programs[setting]);
    }

  private:
    const Array<uint8_t, N> programs;
    const uint8_t channel;
};