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
    ChangeAddress = 0,
    CHANGE_ADDRESS CS_DEPREC("Use ChangeAddress instead") = ChangeAddress,
    /**
     * @brief   Change the offset of the channel number of the element.
     */
    ChangeChannel = 1,
    CHANGE_CHANNEL CS_DEPREC("Use ChangeChannel instead") = ChangeChannel,
    /**
     * @brief   Change the offset of the cable number of the element.
     */
    ChangeCable = 2,
    CHANGE_CABLENB CS_DEPREC("Use ChangeCable instead") = ChangeCable,
};

template <setting_t NumBanks>
class Bank;
class OutputBank;

template <BankType DefaultBankType>
struct OutputBankConfig;

/** 
 * @brief   A struct for selecting the bank of BankableMIDIInput%s and the
 *          bank type.
 */
template <setting_t N>
class BaseBankConfig {
  protected:
    BaseBankConfig(Bank<N> &bank, BankType type) : bank(bank), type(type) {}

  public:
    Bank<N> &bank;
    const BankType type;
};

/// @copydoc BaseBankConfig
template <setting_t N, BankType DefaultBankType = BankType::ChangeAddress>
struct BankConfig : BaseBankConfig<N> {
    BankConfig(Bank<N> &bank, BankType type = DefaultBankType)
        : BaseBankConfig<N>(bank, type) {}
};

/** 
 * @brief   A struct for selecting the bank of BankableMIDIOutput%s and the
 *          bank type.
 */
class BaseOutputBankConfig {
  protected:
    BaseOutputBankConfig(OutputBank &bank, BankType type)
        : bank(bank), type(type) {}

  public:
    template <setting_t N>
    BaseOutputBankConfig(BaseBankConfig<N> config)
        : bank(config.bank), type(config.type) {}

    OutputBank &bank;
    const BankType type;
};

/// @copydoc BaseOutputBankConfig
template <BankType DefaultBankType = BankType::ChangeAddress>
struct OutputBankConfig : BaseOutputBankConfig {
    OutputBankConfig(OutputBank &bank, BankType type = DefaultBankType)
        : BaseOutputBankConfig(bank, type) {}
};

END_CS_NAMESPACE