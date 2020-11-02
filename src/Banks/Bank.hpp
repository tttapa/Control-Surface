/* ✔ */

#pragma once

#include <AH/Containers/LinkedList.hpp>
#include <AH/Debug/Debug.hpp>
#include <AH/Error/Error.hpp>
#include <Selectors/Selectable.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N>
class BankableMIDIInput;

/**
 * @brief   A class for changing the address of BankableMIDIOutput%s.
 */
class OutputBank {
  public:
    /**
     * @brief   Create a new OutputBank object.
     * 
     * @param   tracksPerBank
     *          The number of addresses/tracks to skip for each bank setting.  
     *          Must be strictly positive.
     * @param   initialSelection
     *          The initial bank setting.
     */
    OutputBank(uint8_t tracksPerBank = 1, setting_t initialSelection = 0)
        : tracksPerBank(tracksPerBank), bankSetting(initialSelection) {
        if (tracksPerBank == 0)
            FATAL_ERROR(F("A Bank must have a non-zero number of tracks."),
                        0x4573);
    }

    /**
     * @brief   Select the given bank setting.
     * 
     * @param   setting
     *          The new setting to select.
     */
    void select(setting_t setting) { bankSetting = setting; }

    /**
     * @brief   Get the current bank setting.
     */
    virtual setting_t getSelection() const { return bankSetting; }

    /**
     * @brief   Get the number of tracks per bank.
     * This is the number of addresses/tracks to skip for each bank setting.
     */
    uint8_t getTracksPerBank() const { return tracksPerBank; }

    /**
     * @brief   Get the offset (number of banks times the index of the selected 
     *          bank)
     */
    uint8_t getOffset() const { return getSelection() * getTracksPerBank(); }

  private:
    uint8_t tracksPerBank;
    setting_t bankSetting;
};

/// Callback class for Bankable objects that need to be notified when the
/// active setting of their Bank changes.
class BankSettingChangeCallback
    : public DoublyLinkable<BankSettingChangeCallback> {
    template <setting_t N>
    friend class Bank;

  private:
    /**
     * @brief   A function to be executed each time the bank setting changes.
     * 
     * Think of an LED that indicates whether a track is muted or not. If this 
     * LED is bankable, let's say with 4 tracks per bank, 2 banks, and a base
     * address of 3, then this LED object keeps the state of tracks 3 and 7.
     * When the bank setting is 0, the LED displays the state of track 3, when
     * the bank setting is 1, the LED displays the state of track 7.  
     * To know when to update the LED, this callback is used.
     */
    virtual void onBankSettingChange() {}
};

/**
 * @brief   A class that groups Bankable MIDI Output Elements and 
 *          Bankable MIDI Input Elements, and allows the user to change the
 *          addresses of these elements.
 * 
 * @tparam  N 
 *          The number of banks.
 */
template <setting_t N>
class Bank : public Selectable<N>, public OutputBank {

  public:
    /**
     * @brief   Construct a new Bank object.
     *
     * @param   tracksPerBank
     *          The number of addresses/tracks to skip for each bank setting.  
     *          Must be strictly positive.
     * @param   initialSelection
     *          The initial bank setting.
     */
    Bank(uint8_t tracksPerBank = 1, setting_t initialSelection = 0)
        : Selectable<N>(initialSelection),
          OutputBank(tracksPerBank, initialSelection) {}

    /**
     * @brief   Select the given bank setting.
     * 
     * All Bankable MIDI Input elements that were added to this bank will be
     * updated.
     *
     * @param   bankSetting
     *          The new setting to select.
     */
    void select(setting_t bankSetting) override;

    /**
     * @brief   Get the number of Banks.
     */
    constexpr static uint8_t getNumberOfBanks() { return N; }

  public:
    /**
     * @brief   Add a Bankable MIDI Input Element to the bank.
     * 
     * @param   bankable
     *          The MIDI Input Element to be added.
     */
    void add(BankSettingChangeCallback *bankable);

    /**
     * @brief   Remove a Bankable MIDI Input Element from the bank.
     * 
     * @param   bankable
     *          The MIDI Input Element to be removed.
     */
    void remove(BankSettingChangeCallback *bankable);

  private:
    /**
     * @brief   A linked list of all Bankable MIDI Input Elements that have been
     *          added to this bank, and that should be updated when the bank
     *          setting changes.
     * 
     * The list is updated automatically when Bankable MIDI Input Elements are
     * created or destroyed.
     */
    DoublyLinkedList<BankSettingChangeCallback> inputBankables;
};

END_CS_NAMESPACE

// ---------------------------- Implementations ----------------------------- //

BEGIN_CS_NAMESPACE

template <setting_t N>
void Bank<N>::add(BankSettingChangeCallback *bankable) {
    inputBankables.append(bankable);
}

template <setting_t N>
void Bank<N>::remove(BankSettingChangeCallback *bankable) {
    inputBankables.remove(bankable);
}

template <setting_t N>
void Bank<N>::select(setting_t bankSetting) {
    bankSetting = this->validateSetting(bankSetting);
    OutputBank::select(bankSetting);
    for (BankSettingChangeCallback &e : inputBankables)
        e.onBankSettingChange();
}

END_CS_NAMESPACE