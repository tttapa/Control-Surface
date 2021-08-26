#pragma once

#include <Control_Surface/Control_Surface_Class.hpp>
#include <Def/Def.hpp>
#include <Selectors/Selectable.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N>
class ProgramChanger : public Selectable<N> {
  public:
    ProgramChanger(const Array<uint8_t, N> &programs,
                   MIDIChannelCable channelCN)
        : programs(programs), channelCN(channelCN) {}

    void select(setting_t setting) override {
        setting = this->validateSetting(setting);
        Control_Surface.sendProgramChange(channelCN, programs[setting]);
    }

  private:
    Array<uint8_t, N> programs;
    MIDIChannelCable channelCN;
};

END_CS_NAMESPACE