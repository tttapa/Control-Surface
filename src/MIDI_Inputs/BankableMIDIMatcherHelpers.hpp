#pragma once

#include <Banks/Bank.hpp>
#include <Banks/BankConfig.hpp>
#include <Def/MIDIAddress.hpp>

BEGIN_CS_NAMESPACE

namespace BankableMIDIMatcherHelpers {

/**
 * @brief   Check whether a given address is within a range of given length
 *          starting from the given base address.
 * 
 * @param   tgt
 *          The address to check
 * @param   base
 *          The base address, start of the range.
 * @param   length
 *          The length of the range.
 */
inline bool inRange(uint8_t tgt, uint8_t base, uint8_t length) {
    return (base <= tgt) && (tgt - base < length);
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
 * @param   tgt 
 *          The address to check.
 * @param   base
 *          The base address (the address of bank setting 0).
 * @param   bank
 *          The bank to match the address in.
 * 
 * @note    Equivalent to `matchBankableInRange(toMatch, base, 1)`.
 */
template <uint8_t BankSize>
bool matchBankable(uint8_t tgt, uint8_t base, const Bank<BankSize> &bank) {
    const uint8_t N = BankSize;
    const uint8_t B = bank.getTracksPerBank();
    const int8_t F = bank.getSelectionOffset();
    const uint8_t m = tgt;
    const uint8_t b = base;
    uint8_t diff = m - b - F * B;
    return m >= b + F * B && //
           diff < N * B &&   //
           diff % B == 0;
}

/// @see @ref matchBankableInRange(MIDIAddress,MIDIAddress,BaseBankConfig<BankSize>,uint8_t)
template <uint8_t BankSize>
bool matchBankableInRange(uint8_t tgt, uint8_t base, const Bank<BankSize> &bank,
                          uint8_t rangeLen) {
    const uint8_t R = rangeLen;
    const uint8_t N = BankSize;
    const uint8_t B = bank.getTracksPerBank();
    const int8_t F = bank.getSelectionOffset();
    const uint8_t m = tgt;
    const uint8_t b = base;
    uint8_t diff = m - b - F * B;
    return m >= b + F * B && //
           diff < N * B &&   //
           diff % B < R;
}

/// @see @ref getRangeIndex(MIDIAddress,MIDIAddress,BaseBankConfig<BankSize>)
template <uint8_t BankSize>
uint8_t getRangeIndex(uint8_t tgt, uint8_t base, const Bank<BankSize> &bank) {
    const uint8_t B = bank.getTracksPerBank();
    const int8_t F = bank.getSelectionOffset();
    const uint8_t m = tgt;
    const uint8_t b = base;
    uint8_t diff = m - b - F * B;
    return diff % B;
}

/// @see @ref getBankIndex(MIDIAddress,MIDIAddress,BaseBankConfig<BankSize>)
template <uint8_t BankSize>
uint8_t getBankIndex(uint8_t tgt, uint8_t base, const Bank<BankSize> &bank) {
    const uint8_t B = bank.getTracksPerBank();
    const int8_t F = bank.getSelectionOffset();
    const uint8_t m = tgt;
    const uint8_t b = base;
    uint8_t diff = m - b - F * B;
    return diff / B;
}

/**
 * @brief   Check whether a given address is part of the bank relative to
 *          the base address.
 * 
 * @param   tgt 
 *          The address to check.
 * @param   base
 *          The base address (the address of bank setting 0).
 * @param   config
 *          The bank configuration.
 */
template <uint8_t BankSize>
bool matchBankable(MIDIAddress tgt, MIDIAddress base,
                   BaseBankConfig<BankSize> config) {
    if (!tgt.isValid() || !base.isValid())
        return false;
    switch (config.type) {
        case BankType::CHANGE_ADDRESS: {
            return tgt.getChannel() == base.getChannel() &&
                   tgt.getCableNumber() == base.getCableNumber() &&
                   matchBankable(tgt.getAddress(), base.getAddress(),
                                 config.bank);
        }
        case BankType::CHANGE_CHANNEL: {
            return tgt.getAddress() == base.getAddress() &&
                   tgt.getCableNumber() == base.getCableNumber() &&
                   matchBankable(tgt.getRawChannel(), base.getRawChannel(),
                                 config.bank);
        }
        case BankType::CHANGE_CABLENB: {
            return tgt.getAddress() == base.getAddress() &&
                   tgt.getChannel() == base.getChannel() &&
                   matchBankable(tgt.getRawCableNumber(),
                                 base.getRawCableNumber(), config.bank);
        }
        default: return false; // LCOV_EXCL_LINE
    }
}

/**
 * @brief   Check whether a given address is part of the bank relative to
 *          the base address and within a range with a given length.
 * 
 * @param   tgt 
 *          The address to check.
 * @param   base
 *          The base address (the address of bank setting 0).
 * @param   config
 *          The bank configuration.
 * @param   length
 *          The length of the range.
 */
template <uint8_t BankSize>
bool matchBankableInRange(MIDIAddress tgt, MIDIAddress base,
                          BaseBankConfig<BankSize> config, uint8_t length) {
    if (!tgt.isValid() || !base.isValid())
        return false;
    switch (config.type) {
        case CHANGE_ADDRESS:
            return tgt.getChannel() == base.getChannel() &&
                   tgt.getCableNumber() == base.getCableNumber() &&
                   matchBankableInRange(tgt.getAddress(), base.getAddress(),
                                        config.bank, length);
        case CHANGE_CHANNEL:
            return inRange(tgt.getAddress(), base.getAddress(), length) &&
                   tgt.getCableNumber() == base.getCableNumber() &&
                   matchBankable(tgt.getRawChannel(), base.getRawChannel(),
                                 config.bank);
        case CHANGE_CABLENB:
            return inRange(tgt.getAddress(), base.getAddress(), length) &&
                   tgt.getChannel() == base.getChannel() &&
                   matchBankable(tgt.getRawCableNumber(),
                                 base.getRawCableNumber(), config.bank);
        default: return false;
    }
}

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
    switch (config.type) {
        case BankType::CHANGE_ADDRESS:
            return getBankIndex(target.getAddress(), base.getAddress(),
                                config.bank);
        case BankType::CHANGE_CHANNEL:
            return getBankIndex(target.getRawChannel(), base.getRawChannel(),
                                config.bank);
        case BankType::CHANGE_CABLENB:
            return getBankIndex(target.getRawCableNumber(),
                                base.getRawCableNumber(), config.bank);
        default: return 0; // LCOV_EXCL_LINE
    }
}

/**
 * @brief   Calculate the index in the address range of a given MIDI address,
 *          relative to a base address.
 * 
 * @param   tgt 
 *          The MIDI address to calculate the bank setting of.
 * @param   base
 *          The base address to compare it to (beginning of the range for bank
 *          setting 0).
 * @param   config
 *          The bank configuration to determine the index.
 */
template <uint8_t BankSize>
uint8_t getRangeIndex(MIDIAddress tgt, MIDIAddress base,
                      BaseBankConfig<BankSize> config) {
    return config.type == BankType::CHANGE_ADDRESS
               ? getRangeIndex(tgt.getAddress(), base.getAddress(), config.bank)
               : tgt.getAddress() - base.getAddress();
}

} // namespace BankableMIDIMatcherHelpers

END_CS_NAMESPACE