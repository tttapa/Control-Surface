#pragma once

#include "Bank.h"

enum BankType {
    /**
     * @brief   Change the offset of the channel number of the Bankable.
     */
    CHANGE_CHANNEL = 0,
    /**
     * @brief   Change the offset of the address (i.e. Controller number or
     *          Note number) of the MIDI_Element.
     */
    CHANGE_ADDRESS = 1,
    /**
     * @brief   Change the offset of the cable number of the Bankable 
     *          (experimental).
     * @todo    Implement
     */
    CHANGE_CABLENB = 2,
};

template <setting_t N>
class BankConfig {
  public:
    BankConfig(Bank<N> &bank, BankType type = CHANGE_ADDRESS)
        : bank(bank), type(type) {}
    Bank<N> &bank;
    const BankType type;
};

class OutputBankConfig {
  public:
    OutputBankConfig(OutputBank &bank, BankType type = CHANGE_ADDRESS)
        : bank(bank), type(type) {}
    OutputBank &bank;
    const BankType type;
};