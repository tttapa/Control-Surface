#pragma once

#include "Bank.h"
#include "BankConfigAddressable.hpp"

class BankableMIDIOutputAddressable {
  protected:
    BankableMIDIOutputAddressable(const Bank &bank, Bank::bankType type)
        : bank(bank),
          channelsOrAddressesPerBank(bank.getTracksPerBank() << type) {}

    BankableMIDIOutputAddressable(const BankConfigAddressable &config)
        : BankableMIDIOutputAddressable(config.bank, config.type) {}

  public:
    uint8_t getChannel(uint8_t baseChannel) const {
        return baseChannel + getChannelsPerBank() * getBankSetting();
    }

    uint8_t getAddress(uint8_t baseAddress) const {
        return baseAddress + getAddressesPerBank() * getBankSetting();
    }

    uint8_t getAddressesPerBank() const {
        return channelsOrAddressesPerBank & 0xF;
    }

    uint8_t getChannelsPerBank() const {
        return channelsOrAddressesPerBank >> 4;
    }

    uint8_t getRawBankSetting() const { return bank.getBankSetting(); }

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
    const Bank &bank;
    const uint8_t channelsOrAddressesPerBank;

    constexpr static uint8_t UNLOCKED = 0xFF;
    uint8_t lockedSetting = UNLOCKED;
};