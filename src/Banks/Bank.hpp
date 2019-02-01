#pragma once

#include <Helpers/Debug.hpp>
#include <Helpers/Error.hpp>
#include <Helpers/LinkedList.hpp>
#include <Selectors/Selectable.hpp>
#include <stddef.h>
#include <stdint.h>

template <setting_t N>
class BankableMIDIInput;

class BankableMIDIOutput;
class BankableMIDIOutput;

class OutputBank {
  public:
    OutputBank(uint8_t tracksPerBank = 1, setting_t initialSelection = 0)
        : tracksPerBank(tracksPerBank), bankSetting(initialSelection) {
        if (tracksPerBank == 0) {
            FATAL_ERROR(
                F("Error: A Bank must have a non-zero number of tracks."),
                0x4573);
        }
    }
    void select(setting_t setting) { bankSetting = setting; }
    virtual setting_t getSelection() const { return bankSetting; }
    uint8_t getTracksPerBank() const { return tracksPerBank; }

  private:
    const uint8_t tracksPerBank;
    setting_t bankSetting;
};

/**
 * @brief   A class that groups MIDI_Element%s and allows the user to change
 *          the MIDI addresses (Controller number or Note number) or the MIDI
 *          channels of these elements.
 */
template <setting_t N>
class Bank : public Selectable<N>, public OutputBank {
    friend class BankableMIDIInput<N>;

  public:
    /**
     * @brief   Construct a new Bank object.
     *
     * @param   tracksPerBank
     *          The number of tracks (i.e. addresses or channels) that
     *          are skipped when the bank setting changes.  
     *          Must be greater than zero.
     * @param   initialSelection
     *          The initial bank setting.
     */
    Bank(uint8_t tracksPerBank = 1, setting_t initialSelection = 0)
        : Selectable<N>(initialSelection),
          OutputBank(tracksPerBank, initialSelection) {}

    /**
     * @brief   Set the Bank Setting to the given value.
     * 
     * All BankableMIDIInput will be updated.
     *
     * @param   bankSetting
     *          The new Bank Setting.
     */
    void select(setting_t bankSetting) override;

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

    DoublyLinkedList<BankableMIDIInput<N>> inputBankables;
};

// ---------------------------- Implementations ----------------------------- //

#include <Banks/BankableMIDIInput.hpp>

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