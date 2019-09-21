#include "Bank.hpp"
#include <Def/Def.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A base class for all MIDIOutputElement#s that can have one of many
 *          addresses.
 *          
 * The bank setting determines the address that's being used.
 *          
 * @tparam  N
 *          The number of addresses the element has.
 *          
 * @note    To prevent 'sticky' notes (i.e. a button is pressed, a note on is
 *          sent, the bank is changed, the button is released, and the note off
 *          is sent to a different address, causing the first note to keep on 
 *          playing indefinitely), there must be a way to lock the bank setting
 *          while a note is playing. Then when it is no longer playing, the 
 *          bank setting is unlocked.
 * @invariant `getSelection` and `getRawBankSetting` always return a number in 
 *            the half-open interval @f$\left[0, N\right) \cap \mathbb{N}@f$ .
 */
template <setting_t N>
class ManyAddressesMIDIOutput {
  protected:
    /**
     * @brief   Create a new ManyAddressesMIDIOutput object.
     * 
     * @param   bank
     *          The bank to add this element to.
     */
    ManyAddressesMIDIOutput(const Bank<N> &bank) : bank(bank) {}

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
    const Bank<N> &bank;

    constexpr static setting_t UNLOCKED = NO_SETTING;
    setting_t lockedSetting = UNLOCKED;
};

END_CS_NAMESPACE