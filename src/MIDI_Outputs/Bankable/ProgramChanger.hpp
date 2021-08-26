#pragma once

#include <Banks/BankableAddresses.hpp>
#include <Control_Surface/Control_Surface_Class.hpp>
#include <Def/Def.hpp>
#include <Selectors/Selectable.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

template <setting_t N>
class ProgramChanger : public Selectable<N> {
  public:
    ProgramChanger(OutputBankConfig<> bank, const Array<uint8_t, N> &programs,
                   MIDIChannelCable channelCN)
        : bank(bank), programs(programs), channelCN(channelCN) {}

    void select(setting_t setting) override {
        setting = this->validateSetting(setting);
        MIDIAddress addr {programs[setting], channelCN};
        Control_Surface.sendProgramChange(addr + bank.getAddressOffset());
    }

  private:
    OutputBankableMIDIAddress bank;
    Array<uint8_t, N> programs;
    MIDIChannelCable channelCN;
};

} // namespace Bankable

END_CS_NAMESPACE