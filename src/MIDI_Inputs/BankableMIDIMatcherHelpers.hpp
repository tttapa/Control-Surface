#pragma once

#include <Banks/Bank.hpp>
#include <Banks/BankConfig.hpp>

BEGIN_CS_NAMESPACE

namespace BankableMIDIMatcherHelpers {

/**
 * @brief   Calculate the bank setting of a given MIDI address, relative to
 *          a base address.
 * 
 * @param   target 
 *          The MIDI address to calculate the bank setting of.
 * @param   base
 *          The base address to compare it to (the address of bank setting 0).
 * @param   config
 *          The bank configuration to determine the index.
 */
template <uint8_t BankSize>
uint8_t getBankIndex(MIDIAddress target, MIDIAddress base,
                     BaseBankConfig<BankSize> config) {
    auto nt = config.bank.getTracksPerBank();
    switch (config.type) {
        case BankType::CHANGE_ADDRESS:
            return (target.getAddress() - base.getAddress()) / nt;
        case BankType::CHANGE_CHANNEL:
            return (target.getRawChannel() - base.getRawChannel()) / nt;
        case BankType::CHANGE_CABLENB:
            return (target.getRawCableNumber() - base.getRawCableNumber()) / nt;
        default: return 0;
    }
}

/**
 * @brief   Check if the given address is part of the bank relative to the
 *          base address.
 * 
 * Consider the following example:  
 * A Bank with 4 tracks per bank (T), 2 bank settings (N), 
 * and a base address of 3.
 * 
 * ~~~
 * 0   1   2   3   4   5   6   7   8   9  10  11  12  ...
 * ☐   ☐   ☐   ☒   ☐   ☐   ☐   ☒   ☐   ☐   ☐   ☐   ☐  ...
 * ~~~
 * 
 * Addresses before the base adddress are not matched (0, 1, 2).  
 * Addresses after N * T are not matched (8, 9, 10, 11, 12).  
 * Addresses with a distance to the base address that is not a multiple of N
 * are not matched (4, 5, 6).
 * 
 * @param   toMatch 
 *          The address to check.
 * @param   base
 *          The base address (the address of bank setting 0).
 * 
 * @note    Equivalent to `matchBankableInRange(toMatch, base, 1)`.
 */
template <uint8_t BankSize>
bool matchBankable(uint8_t toMatch, uint8_t base, const Bank<BankSize> &bank) {
    uint8_t diff = toMatch - base;
    return toMatch >= base &&                           //
           diff < BankSize * bank.getTracksPerBank() && //
           diff % bank.getTracksPerBank() == 0;
}

/**
     * @brief   Check whether a given address is part of the bank relative to
     *          the base address.
     * 
     * @param   toMatch 
     *          The address to check.
     * @param   base
     *          The base address (the address of bank setting 0).
     */
template <uint8_t BankSize>
bool matchBankable(MIDIAddress toMatch, MIDIAddress base,
                   BaseBankConfig<BankSize> config) {
    if (!toMatch.isValid() || !base.isValid())
        return false;
    switch (config.type) {
        case BankType::CHANGE_ADDRESS: {
            return toMatch.getChannel() == base.getChannel() &&
                   toMatch.getCableNumber() == base.getCableNumber() &&
                   matchBankable(toMatch.getAddress(), base.getAddress(),
                                 config.bank);
        }
        case BankType::CHANGE_CHANNEL: {
            return toMatch.getAddress() == base.getAddress() &&
                   toMatch.getCableNumber() == base.getCableNumber() &&
                   matchBankable(toMatch.getRawChannel(), base.getRawChannel(),
                                 config.bank);
        }
        case BankType::CHANGE_CABLENB: {
            return toMatch.getAddress() == base.getAddress() &&
                   toMatch.getChannel() == base.getChannel() &&
                   matchBankable(toMatch.getRawCableNumber(),
                                 base.getRawCableNumber(), config.bank);
        }
        default: return false;
    }
}

} // namespace BankableMIDIMatcherHelpers

END_CS_NAMESPACE