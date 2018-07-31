#pragma once

#include "Bank.h"

class BankableMIDIOutput {
  protected:
    BankableMIDIOutput(const OutputBank &bank)
        : bank(bank), channelsPerBank(bank.getTracksPerBank()) {}

  public:
    uint8_t getChannel(uint8_t baseChannel) const {
        return baseChannel + getChannelsPerBank() * getSelection();
    }

    uint8_t getChannelsPerBank() const { return channelsPerBank; }

    uint8_t getRawBankSetting() const { return bank.getSelection(); }

    uint8_t getSelection() const {
        return lockedSetting == UNLOCKED ? getRawBankSetting() : lockedSetting;
    }

  protected:
    void lock() {
        if (lockedSetting == UNLOCKED)
            lockedSetting = getRawBankSetting();
    }

    void unlock() { lockedSetting = UNLOCKED; }

  private:
    const OutputBank &bank;
    const uint8_t channelsPerBank;

    constexpr static uint8_t UNLOCKED = 0xFF;
    uint8_t lockedSetting = UNLOCKED;
};