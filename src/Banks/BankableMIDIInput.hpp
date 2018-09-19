#pragma once

#include "Bank.h"
#include "BankConfig.hpp"
#include <Helpers/Debug.hpp>
#include <Helpers/LinkedList.hpp>
#include <Helpers/MIDICNChannelAddress.hpp>

template <setting_t N>
class BankableMIDIInput : public DoublyLinkable<BankableMIDIInput<N>> {
    friend class Bank<N>;

  protected:
    BankableMIDIInput(Bank<N> &bank, BankType type) : bank(bank), type(type) {
        bank.add(this);
    }

    BankableMIDIInput(const BankConfig<N> &config)
        : BankableMIDIInput<N>(config.bank, config.type) {}

  public:
    virtual ~BankableMIDIInput() { bank.remove(this); }

    setting_t getSelection() const { return bank.getSelection(); }

    uint8_t getIndex(const MIDICNChannelAddress &target,
                     const MIDICNChannelAddress &base) const {
        switch (type) {
            case CHANGE_ADDRESS:
                return (target.getAddress() - base.getAddress()) /
                       bank.getTracksPerBank();
            case CHANGE_CHANNEL:
                return (target.getChannel() - base.getChannel()) /
                       bank.getTracksPerBank();
            case CHANGE_CN:
                return (target.getCableNumber() - base.getCableNumber()) /
                       bank.getTracksPerBank();
            default: return 0;
        }
    }

  protected:
    bool match(const MIDICNChannelAddress &target,
               const MIDICNChannelAddress &base) const {
        switch (type) {
            case CHANGE_ADDRESS: {
                int8_t diff = target.getAddress() - base.getAddress();
                return target.getChannel() == base.getChannel() &&
                       target.getCableNumber() == base.getCableNumber() &&
                       diff >= 0 && diff <= N * bank.getTracksPerBank() &&
                       diff % bank.getTracksPerBank() == 0;
            }
            case CHANGE_CHANNEL: {
                int8_t diff = target.getChannel() - base.getChannel();
                return target.getAddress() == base.getAddress() &&
                       target.getCableNumber() == base.getCableNumber() &&
                       diff >= 0 && diff <= N * bank.getTracksPerBank() &&
                       diff % bank.getTracksPerBank() == 0;
            }
            case CHANGE_CN: {
                int8_t diff = target.getCableNumber() - base.getCableNumber();
                return target.getAddress() == base.getAddress() &&
                       target.getChannel() == base.getChannel() && diff >= 0 &&
                       diff <= N * bank.getTracksPerBank() &&
                       diff % bank.getTracksPerBank() == 0;
            }
            default: return false;
        }
    }

  private:
    Bank<N> &bank;
    const BankType type;

    virtual void onBankSettingChange() const {}
};