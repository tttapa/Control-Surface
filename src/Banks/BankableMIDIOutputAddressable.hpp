#pragma once

#include "Bank.h"
#include "BankConfigAddressable.hpp"

class BankableMIDIOutputAddressable {
  protected:
    BankableMIDIOutputAddressable(const OutputBank &bank, BankType type)
        : bank(bank),
          channelsOrAddressesPerBank(bank.getTracksPerBank() << type) {}

    BankableMIDIOutputAddressable(const OutputBankConfigAddressable &config)
         : BankableMIDIOutputAddressable(config.bank, config.type) {}

  public:
    uint8_t getChannel(uint8_t baseChannel) const {
        return baseChannel + getChannelsPerBank() * getSelection();
    }

    uint8_t getAddress(uint8_t baseAddress) const {
        return baseAddress + getAddressesPerBank() * getSelection();
    }

    uint8_t getAddressesPerBank() const {
        return channelsOrAddressesPerBank & 0xF;
    }

    uint8_t getChannelsPerBank() const {
        return channelsOrAddressesPerBank >> 4;
    }

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
    const uint8_t channelsOrAddressesPerBank;

    constexpr static uint8_t UNLOCKED = 0xFF;
    uint8_t lockedSetting = UNLOCKED;
};