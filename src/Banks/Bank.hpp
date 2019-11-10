/* ✔ */

#pragma once

#include <AH/Debug/Debug.hpp>
#include <AH/Error/Error.hpp>
#include <AH/Containers/LinkedList.hpp>
#include <Selectors/Selectable.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N>
class BankableMIDIInput;

class BankableMIDIOutput;

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

/**
 * @brief   A class that groups Bankable BankableMIDIOutput%s and 
 *          BankableMIDIInput%s, and allows the user to change the addresses 
 *          of these elements.
 * 
 * @tparam  N 
 *          The number of banks.
 */
template <setting_t N>
class Bank : public Selectable<N>, public OutputBank {
    friend class BankableMIDIInput<N>;

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
     * All BankableMIDIInput%s will be updated.
     *
     * @param   bankSetting
     *          The new setting to select.
     */
    void select(setting_t bankSetting) override;

    /**
     * @brief   Get the number of Banks.
     */
    constexpr static uint8_t getNumberOfBanks() { return N; }

  private:
    /**
     * @brief   Add a BankableMIDIInput to the bank.
     * 
     * This method is called in the BankableMIDIInput constructor.
     *
     * @param   bankable
     *          The BankableMIDIInput to be added.
     */
    void add(BankableMIDIInput<N> *bankable);

    /**
     * @brief   Remove a BankableMIDIInput from the bank.
     * 
     * This method is called in the BankableMIDIInput destructor.
     *
     * @param   bankable
     *          The BankableMIDIInput to be removed.
     */
    void remove(BankableMIDIInput<N> *bankable);

    /**
     * @brief   A linked list of all BankableMIDIInput elements that have been
     *          added to this bank, and that should be updated when the bank
     *          setting changes.
     * 
     * The list is updated automatically when BankableMIDIInput elements are
     * created or destroyed.
     */
    DoublyLinkedList<BankableMIDIInput<N>> inputBankables;
};

END_CS_NAMESPACE

// ---------------------------- Implementations ----------------------------- //

#include <Banks/BankableMIDIInput.hpp>

BEGIN_CS_NAMESPACE

template <setting_t N>
void Bank<N>::add(BankableMIDIInput<N> *bankable) {
    inputBankables.append(bankable);
}

template <setting_t N>
void Bank<N>::remove(BankableMIDIInput<N> *bankable) {
    inputBankables.remove(bankable);
}

template <setting_t N>
void Bank<N>::select(setting_t bankSetting) {
    bankSetting = this->validateSetting(bankSetting);
    OutputBank::select(bankSetting);
    for (BankableMIDIInput<N> &e : inputBankables)
        e.onBankSettingChange();
}

END_CS_NAMESPACE