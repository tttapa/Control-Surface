#include "Bank.h"
#include <MIDI_Outputs/Abstract_MIDI_Output_Addressable.hpp>

class BankableMIDIOutputAddressable
    : public virtual Abstract_MIDI_Output_Addressable {
    friend class Bank;

  public:
    uint8_t getChannel(uint8_t baseChannel) const override {
        return baseChannel + getChannelsPerBank() * getBankSetting();
    }

    uint8_t getAddress(uint8_t baseAddress) const override {
        return baseAddress + getAddressesPerBank() * getBankSetting();
    }

    uint8_t getAddressesPerBank() const { return addressesPerBank; }

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
    void lock() { lockedSetting = getRawBankSetting(); }

    void unlock() { lockedSetting = UNLOCKED; }

  private:
    Bank *bank = nullptr;
    uint8_t channelsPerBank = 0;
    uint8_t addressesPerBank = 0;

    void setBank(Bank *bank, Bank::bankType type) {
        if (this->bank != nullptr) {
            DEBUGFN(F("Error: This Bankable already has a Bank."));
            ERROR(return );
        }
        this->bank = bank;
        if (type == Bank::CHANGE_ADDRESS)
            addressesPerBank = bank->getTracksPerBank();
        else
            channelsPerBank = bank->getTracksPerBank();
    }

    void removeBank() {
        this->bank = nullptr;
        addressesPerBank = 0;
        channelsPerBank = 0;
    }

    constexpr static uint8_t UNLOCKED = 0xFF;
    uint8_t lockedSetting = UNLOCKED;
};