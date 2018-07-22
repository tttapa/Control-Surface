#pragma once

#include "Bank.h"

class BankableMIDIOutput {
    friend class Bank;

  public:
    uint8_t getChannel(uint8_t baseChannel) const {
        return baseChannel + getChannelsPerBank() * getBankSetting();
    }

    uint8_t getChannelsPerBank() const { return channelsPerBank; }

    uint8_t getRawBankSetting() const {
        if (bank == nullptr) {
            DEBUGFN(F("Error: This Bankable element does not have a bank."));
            ERROR(return 0);
        }
        return bank->getBankSetting();
    }

    uint8_t getBankSetting() const {
        return lockedSetting == UNLOCKED ? getRawBankSetting() : lockedSetting;
    }

  protected:
    void lock() {
        if (lockedSetting == UNLOCKED)
            lockedSetting = getRawBankSetting();
    }

    void unlock() { lockedSetting = UNLOCKED; }

  private:
    Bank *bank = nullptr;
    uint8_t channelsPerBank = 0;

    void setBank(Bank *bank) {
        if (this->bank != nullptr) {
            DEBUGFN(F("Error: This Bankable already has a Bank."));
            ERROR(return );
        }
        this->bank = bank;
        channelsPerBank = bank->getTracksPerBank();
    }

    void removeBank() {
        this->bank = nullptr;
        channelsPerBank = 0;
    }

    constexpr static uint8_t UNLOCKED = 0xFF;
    uint8_t lockedSetting = UNLOCKED;
};