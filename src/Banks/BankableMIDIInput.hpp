#pragma once

#include "Bank.h"
#include "BankConfig.hpp"
#include <Def/MIDICNChannelAddress.hpp>
#include <Helpers/Debug.hpp>
#include <Helpers/LinkedList.hpp>

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
                return (target.getChannel().getRaw() -
                        base.getChannel().getRaw()) /
                       bank.getTracksPerBank();
            case CHANGE_CN:
                return (target.getCableNumber() - base.getCableNumber()) /
                       bank.getTracksPerBank();
            default: return 0;
        }
    }

  protected:
    /**
     * @note    Equivalent to `matchBankableInRange(toMatch, base, 1)`.
     */
    bool matchBankable(uint8_t toMatch, uint8_t base) const {
        uint8_t diff = toMatch - base;
        return toMatch >= base && diff <= N * bank.getTracksPerBank() &&
               diff % bank.getTracksPerBank() == 0;
    }

    bool matchBankableInRange(uint8_t toMatch, uint8_t base,
                              uint8_t length) const {
        uint8_t diff = toMatch - base;
        return toMatch >= base && diff <= N * bank.getTracksPerBank() &&
               diff % bank.getTracksPerBank() < length;
    }

    static bool inRange(uint8_t toMatch, uint8_t base, uint8_t length) {
        return (base <= toMatch) && (toMatch - base < length);
    }

    bool matchBankable(const MIDICNChannelAddress &toMatch,
                       const MIDICNChannelAddress &base) const {
        if (!toMatch.isValid() || !base.isValid())
            return false;
        switch (type) {
            case CHANGE_ADDRESS: {
                return toMatch.getChannel() == base.getChannel() &&
                       toMatch.getCableNumber() == base.getCableNumber() &&
                       matchBankable(toMatch.getAddress(), base.getAddress());
            }
            case CHANGE_CHANNEL: {
                return toMatch.getAddress() == base.getAddress() &&
                       toMatch.getCableNumber() == base.getCableNumber() &&
                       matchBankable(toMatch.getChannel().getRaw(),
                                     base.getChannel().getRaw());
            }
            case CHANGE_CN: {
                return toMatch.getAddress() == base.getAddress() &&
                       toMatch.getChannel() == base.getChannel() &&
                       matchBankable(toMatch.getCableNumber(),
                                     base.getCableNumber());
            }
            default: return false;
        }
    }

    bool matchBankableAddressInRange(const MIDICNChannelAddress &toMatch,
                                     const MIDICNChannelAddress &base,
                                     uint8_t length) const {
        if (!toMatch.isValid() || !base.isValid())
            return false;
        switch (type) {
            case CHANGE_ADDRESS: {
                return toMatch.getChannel() == base.getChannel() &&
                       toMatch.getCableNumber() == base.getCableNumber() &&
                       matchBankableInRange(toMatch.getAddress(),
                                            base.getAddress(), length);
            }
            case CHANGE_CHANNEL: {
                return inRange(toMatch.getAddress(), base.getAddress(),
                               length) &&
                       toMatch.getCableNumber() == base.getCableNumber() &&
                       matchBankable(toMatch.getChannel(), base.getChannel());
            }
            case CHANGE_CN: {
                return inRange(toMatch.getAddress(), base.getAddress(),
                               length) &&
                       toMatch.getChannel() == base.getChannel() &&
                       matchBankable(toMatch.getCableNumber(),
                                     base.getCableNumber());
            }
            default: return false;
        }
    }

  private:
    Bank<N> &bank;
    const BankType type;

    virtual void onBankSettingChange() const {}
};