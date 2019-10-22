/* ✔ */

#pragma once

#include <Def/Def.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   An enumeration of the different bank types.
 */
enum BankType {
    /**
     * @brief   Change the offset of the address (i.e. Controller number or
     *          Note number) of the element.
     */
    CHANGE_ADDRESS = 0,
    /**
     * @brief   Change the offset of the channel number of the element.
     */
    CHANGE_CHANNEL = 1,
    /**
     * @brief   Change the offset of the cable number of the element 
     *          (experimental).
     * @todo    Implement
     */
    CHANGE_CABLENB = 2,
};

template <setting_t N>
class Bank;
class OutputBank;

/** 
 * @brief   A struct for selecting the bank of BankableMIDIInput%s and the
 *          bank type.
 */
template <setting_t N>
struct BankConfig {
    BankConfig(Bank<N> &bank, BankType type = CHANGE_ADDRESS)
        : bank(bank), type(type) {}
    Bank<N> &bank;
    const BankType type;
};

/** 
 * @brief   A struct for selecting the bank of BankableMIDIOutput%s and the
 *          bank type.
 */
struct OutputBankConfig {
    OutputBankConfig(OutputBank &bank, BankType type = CHANGE_ADDRESS)
        : bank(bank), type(type) {}
    OutputBank &bank;
    const BankType type;
};

END_CS_NAMESPACE