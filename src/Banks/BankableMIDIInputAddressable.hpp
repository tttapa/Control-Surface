#pragma once

#include "Bank.h"
#include "BankConfigAddressable.hpp"
#include <Helpers/Debug.hpp>

template <setting_t N>
class BankableMIDIInputAddressable {
    friend class Bank<N>;

  protected:
    BankableMIDIInputAddressable(Bank<N> &bank, BankType type)
        : bank(bank),
          channelsOrAddressesPerBank(bank.getTracksPerBank() << type) {
        bank.add(*this);
    }

    BankableMIDIInputAddressable(const BankConfigAddressable<N> &config)
        : BankableMIDIInputAddressable<N>(config.bank, config.type) {}

  public:
    virtual ~BankableMIDIInputAddressable() {
        bank.remove(this);
    }

    uint8_t getAddressesPerBank() const {
        return channelsOrAddressesPerBank & 0xF;
    }

    uint8_t getChannelsPerBank() const {
        return channelsOrAddressesPerBank >> 4;
    }

    uint8_t getIndex(uint8_t targetChannel, uint8_t targetAddress,
                     uint8_t baseChannel, uint8_t baseAddress) const {
        return getAddressesPerBank()
                   ? (targetAddress - baseAddress) / getAddressesPerBank()
                   : (targetChannel - baseChannel) / getChannelsPerBank();
    }

    uint8_t getSelection() const { return bank.getSelection(); }

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
    Bank<N> &bank;
    BankableMIDIInputAddressable<N> *next = nullptr;
    BankableMIDIInputAddressable<N> *previous = nullptr;
    const uint8_t channelsOrAddressesPerBank;

    virtual void onBankSettingChange() const {}
    void onBankSettingChangeAll() const {
        for (const BankableMIDIInputAddressable<N> *e = this; e; e = e->next)
            e->onBankSettingChange();
    }

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};