#pragma once

#include "Bank.h"

enum BankType {
    /**
     * @brief   Change the offset of the channel number of the Bankable.
     */
    CHANGE_CHANNEL = 4,
    /**
     * @brief   Change the offset of the address (i.e. Controller number or
     *          Note number) of the MIDI_Element.
     */
    CHANGE_ADDRESS = 0
};

template <setting_t N>
class BankConfigAddressable {
  public:
    BankConfigAddressable(Bank<N> &bank, BankType type = CHANGE_ADDRESS)
        : bank(bank), type(type) {}
    Bank<N> &bank;
    const BankType type;
};

class OutputBankConfigAddressable {
  public:
    OutputBankConfigAddressable(OutputBank &bank,
                                BankType type = CHANGE_ADDRESS)
        : bank(bank), type(type) {}
    OutputBank &bank;
    const BankType type;
};