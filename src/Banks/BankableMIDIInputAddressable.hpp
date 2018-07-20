#pragma once

#include "Bank.h"
#include <Helpers/Debug.hpp>

class BankableMIDIInputAddressable {
    friend class Bank;

  public:
    virtual ~BankableMIDIInputAddressable() { bank->remove(this); }

    uint8_t getChannel(uint8_t baseChannel) const {
        return baseChannel + getChannelsPerBank() * getBankSetting();
    }

    uint8_t getAddress(uint8_t baseAddress) const {
        return baseAddress + getAddressesPerBank() * getBankSetting();
    }

    uint8_t getAddressesPerBank() const { return addressesPerBank; }

    uint8_t getChannelsPerBank() const { return channelsPerBank; }

    uint8_t getAddressIndex(uint8_t targetAddress, uint8_t baseAddress) const {
        if (getAddressesPerBank() == 0) {
            DEBUGFN(F("Error: This Bankable element is not banked with type "
                      "CHANGE_ADDRESS."));
            ERROR(return 0);
        }
        return (targetAddress - baseAddress) / getAddressesPerBank();
    }

    uint8_t getChannelIndex(uint8_t targetChannel, uint8_t baseChannel) const {
        if (getChannelsPerBank() == 0) {
            DEBUGFN(F("Error: This Bankable element is not banked with type "
                      "CHANGE_CHANNEL."));
            ERROR(return 0);
        }
        return (targetChannel - baseChannel) / getChannelsPerBank();
    }

    uint8_t getIndex(uint8_t targetChannel, uint8_t targetAddress,
                     uint8_t baseChannel, uint8_t baseAddress) const {
        return getAddressesPerBank()
                   ? getAddressIndex(targetAddress, baseAddress)
                   : getChannelIndex(targetChannel, baseChannel);
    }

    uint8_t getBankSetting() const {
        if (bank == nullptr) {
            DEBUGFN(F("Error: This Bankable element does not have a bank."));
            ERROR(return 0);
        }
        return bank->getBankSetting();
    }

  protected:
    bool matchAddress(uint8_t targetAddress, uint8_t baseAddress,
                      uint8_t numberOfBanks) const {
        DEBUGFN(F("target address = ")
                << hex << +targetAddress << F(", base address = ")
                << +baseAddress << dec << F(", number of banks = ")
                << numberOfBanks);
        if (getAddressesPerBank() == 0)
            return targetAddress == baseAddress;
        int8_t addressDiff = targetAddress - baseAddress;
        return (addressDiff >= 0) &&
               (addressDiff < numberOfBanks * getAddressesPerBank()) &&
               (addressDiff % getAddressesPerBank() == 0);
    }

    bool matchChannel(uint8_t targetChannel, uint8_t baseChannel,
                      uint8_t numberOfBanks) const {
        DEBUGFN(F("target channel = ")
                << hex << targetChannel << F(", base channel = ") << baseChannel
                << dec << F(", number of banks = ") << numberOfBanks);
        if (getChannelsPerBank() == 0)
            return targetChannel == baseChannel;
        int8_t channelDiff = targetChannel - baseChannel;
        return (channelDiff >= 0) &&
               (channelDiff < numberOfBanks * getChannelsPerBank()) &&
               (channelDiff % getChannelsPerBank() == 0);
    }

  private:
    Bank *bank = nullptr;
    BankableMIDIInputAddressable *next = nullptr;
    BankableMIDIInputAddressable *previous = nullptr;
    uint8_t channelsPerBank = 0;
    uint8_t addressesPerBank = 0;

    virtual void onBankSettingChange() const {}
    void onBankSettingChangeAll() const {
        for (const BankableMIDIInputAddressable *e = this; e; e = e->next)
            e->onBankSettingChange();
    }

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

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};