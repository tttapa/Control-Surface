/* ✔ */

#pragma once

#include "Bank.hpp"
#include "BankConfig.hpp"
#include <Def/MIDIAddress.hpp>

BEGIN_CS_NAMESPACE

/// Class for keeping track of the active bank, and allows locking of the
/// bank setting.
///
/// @see @ref OutputBankableMIDIAddress
class OutputBankableMIDIAddress_Base {
  protected:
    /**
     * @brief   Constructor.
     * 
     * @param   bank
     *          The bank to add this element to.
     */
    OutputBankableMIDIAddress_Base(const OutputBank &bank) : bank(bank) {}

  public:
    /** 
     * @brief   Get the actual bank setting (no matter whether the element is 
     *          locked or not).
     */
    setting_t getRawBankSetting() const { return bank.getSelection(); }

    /**
     * @brief   Get the bank setting.
     * 
     * If the element is locked, the bank setting from the moment it was locked
     * is returned.
     */
    setting_t getSelection() const {
        return lockedSetting == UNLOCKED ? getRawBankSetting() : lockedSetting;
    }

    /**
     * @brief   Lock the bank setting.
     * 
     * As long as it's locked, `getSelection` will return the current setting,
     * independent from the actual bank setting.
     */
    void lock() {
        if (lockedSetting == UNLOCKED)
            lockedSetting = getRawBankSetting();
    }

    /**
     * @brief   Unlock the bank setting.
     * 
     * After unlocking, @ref getSelection will return the actual bank setting 
     * again.
     */
    void unlock() { lockedSetting = UNLOCKED; }

  protected:
    const OutputBank &bank;

  private:
    constexpr static setting_t UNLOCKED = NO_SETTING;
    setting_t lockedSetting = UNLOCKED;
};

/**
 * @brief   A base class for all MIDIOutputElement%s that can be banked.
 * 
 * @note    These elements don't have to be updated when the bank setting is 
 *          changed, because they poll the bank setting each time they send a
 *          MIDI event.  
 *          They are not added to the bank, they just keep a reference to the 
 *          bank they're a part of.  
 * 
 * @note    To prevent 'sticky' notes (i.e. a button is pressed, a note on is
 *          sent, the bank is changed, the button is released, and the note off
 *          is sent to a different address, causing the first note to keep on 
 *          playing indefinitely), there must be a way to lock the bank setting
 *          while a note is playing. Then when it is no longer playing, the 
 *          bank setting is unlocked.
 */
class OutputBankableMIDIAddress : public OutputBankableMIDIAddress_Base {
  public:
    /**
     * @brief   Create a new OutputBankableMIDIAddress object.
     * 
     * @param   bank
     *          The bank to add this element to.
     * @param   type
     *          What address type to change (address, channel or cable number).
     */
    OutputBankableMIDIAddress(const OutputBank &bank, BankType type)
        : OutputBankableMIDIAddress_Base(bank), type(type) {}

    /**
     * @brief   Create a new OutputBankableMIDIAddress object.
     * 
     * @param   config
     *          The bank and address type to change.
     * 
     * @see     OutputBankableMIDIAddress::OutputBankableMIDIAddress(Bank<N> &, BankType)
     */
    OutputBankableMIDIAddress(BaseOutputBankConfig config)
        : OutputBankableMIDIAddress(config.bank, config.type) {}

    /** 
     * @brief   Get the offset relative to the base address.
     */
    RelativeMIDIAddress getAddressOffset(setting_t bankindex) const {
        int8_t offset = bank.getOffsetOfSetting(bankindex);
        switch (type) {
            case CHANGE_ADDRESS: return {offset, 0, 0};
            case CHANGE_CHANNEL: return {0, offset, 0};
            case CHANGE_CABLENB: return {0, 0, offset};
            default: return {};
        }
    }
    /// @copydoc getAddressOffset(setting_t) const
    RelativeMIDIAddress getAddressOffset() const {
        return getAddressOffset(getSelection());
    }

  private:
    const BankType type;
};

/**
 * @brief   A base class for all MIDIOutputElement%s that can have one of many
 *          addresses.
 *          
 * The bank setting determines the address that's being used.
 *          
 * @note    To prevent 'sticky' notes (i.e. a button is pressed, a note on is
 *          sent, the bank is changed, the button is released, and the note off
 *          is sent to a different address, causing the first note to keep on 
 *          playing indefinitely), there must be a way to lock the bank setting
 *          while a note is playing. Then when it is no longer playing, the 
 *          bank setting is unlocked.
 * 
 * @invariant `getSelection` and `getRawBankSetting` always return a number in 
 *            the half-open interval 
 *            @f$ \left[0, \mathrm{bank.getNumberOfBanks()}\right) \cap 
 *            \mathbb{N} @f$.
 */
class ManyAddresses_Base : public OutputBankableMIDIAddress_Base {
  public:
    /**
     * @brief   Constructor.
     * 
     * @param   bank
     *          The bank to add this element to.
     * @tparam  NumBanks
     *          The number of bank settings @p bank has.
     */
    template <uint8_t NumBanks>
    ManyAddresses_Base(const Bank<NumBanks> &bank)
        : OutputBankableMIDIAddress_Base(bank) {}
};

END_CS_NAMESPACE