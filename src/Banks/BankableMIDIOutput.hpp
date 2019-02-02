/* ✔ */

#pragma once

#include "Bank.hpp"
#include "BankConfig.hpp"
#include <Def/MIDICNChannelAddress.hpp>

/**
 * @brief   A base class for all MIDIOutputElement#s that can be banked.
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
class BankableMIDIOutput {
  protected:
    /**
     * @brief   Create a new BankableMIDIOutput object.
     * 
     * @param   bank
     *          The bank to add this element to.
     * @param   type
     *          What address type to change (address, channel or cable number).
     */
    BankableMIDIOutput(const OutputBank &bank, BankType type)
        : bank(bank), type(type) {}

    /**
     * @brief   Create a new BankableMIDIOutput object.
     * 
     * @param   config
     *          The bank and address type to change.
     * 
     * @see     BankableMIDIOutput::BankableMIDIOutput(Bank<N> &, BankType)
     */
    BankableMIDIOutput(const OutputBankConfig &config)
        : BankableMIDIOutput(config.bank, config.type) {}

  public:
    /** 
     * @brief   Get the offset relative to the base address.
     */
    RelativeMIDICNChannelAddress getAddressOffset() const {
        int8_t offset = getSelection() * bank.getTracksPerBank();
        switch (type) {
            case CHANGE_ADDRESS: return {offset, 0, 0};
            case CHANGE_CHANNEL: return {0, offset, 0};
            case CHANGE_CABLENB: return {0, 0, offset};
            default: return {};
        }
    }

    /** 
     * @brief   Get the actual bank setting (no matter wheter the element is 
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

  protected:
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
     * After unlocking, `getSelection` will return the actual bank setting 
     * again.
     */
    void unlock() { lockedSetting = UNLOCKED; }

  private:
    const OutputBank &bank;
    const BankType type;

    constexpr static setting_t UNLOCKED = NO_SETTING;
    setting_t lockedSetting = UNLOCKED;
};