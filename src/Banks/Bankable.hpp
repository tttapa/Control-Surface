#pragma once

#include "../Helpers/Debug.hpp"
#include "Bank.h"

class Bankable {
    friend class Bank;

  public:
    virtual ~Bankable() { bank->remove(this); }

    uint8_t getChannel(uint8_t baseChannel) const {
        return baseChannel + getChannelsPerBank() * getBankSetting();
    }

    uint8_t getAddress(uint8_t baseAddress) const {
        return baseAddress + getAddressesPerBank() * getBankSetting();
    }

    uint8_t getAddressesPerBank() const { return addressesPerBank; }

    uint8_t getChannelsPerBank() const { return channelsPerBank; }

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

    uint8_t getBankSetting() const { return bank->getBankSetting(); }

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
    Bankable *next = nullptr;
    Bankable *previous = nullptr;
    uint8_t channelsPerBank = 0;
    uint8_t addressesPerBank = 0;

    virtual void onBankSettingChange() const {}
    void onBankSettingChangeAll() const {
        for (const Bankable *e = this; e; e = e->next)
            e->onBankSettingChange();
    }

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};