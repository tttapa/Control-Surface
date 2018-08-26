#pragma once

#include <Helpers/Debug.hpp>
#include <Helpers/Error.hpp>
#include <Helpers/LinkedList.hpp>
#include <Selectors/Selectable.hpp>
#include <stddef.h>
#include <stdint.h>

template <setting_t N>
class BankableMIDIInputAddressable;

class BankableMIDIOutputAddressable;
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
 *
 * ### In general
 * `Bank::CHANGE_CHANNEL`
 * @f$ c = c_0 + t · s @f$ &emsp; where @f$c@f$ is the effective channel,
 * @f$c_0@f$ is the base channel of the MIDI_Element, @f$t@f$ is the number of
 * tracks per bank, and @f$s@f$ is the current bank setting.
 *
 * `Bank::CHANGE_ADDRESS`
 * @f$ a = a_0 + t · s @f$ &emsp; where @f$a@f$ is the effective address,
 * @f$a_0@f$ is the base address of the MIDI_Element, @f$t@f$ is the number of
 * tracks per bank, and @f$s@f$ is the current bank setting.
 */
template <setting_t N>
class Bank : public Selectable<N>, public OutputBank {
    friend class BankableMIDIInputAddressable<N>;

  public:
    /**
     * @brief   Construct a new Bank object.
     *
     * @param   tracksPerBank
     *          The number of tracks (i.e. addresses or channels) that
     *          are skipped when the bank setting changes.
     */
    Bank(uint8_t tracksPerBank = 1, setting_t initialSelection = 0)
        : Selectable<N>(initialSelection),
          OutputBank(tracksPerBank, initialSelection) {}

    /**
     * @brief   Set the Bank Setting
     *
     * @param   bankSetting
     *          The new Bank Setting.
     */
    void select(setting_t bankSetting) override;

  private:
    /**
     * @brief   Add a Bankable to the bank.
     *
     * @param   bankable
     *          The Bankable to be added.
     */
    void add(BankableMIDIInputAddressable<N> *bankable);

    void remove(BankableMIDIInputAddressable<N> *bankable);

    DoublyLinkedList<BankableMIDIInputAddressable<N>> inputBankables;
};

// ---------------------------- Implementations ----------------------------- //

#include <Banks/BankableMIDIInputAddressable.hpp>

template <setting_t N>
void Bank<N>::add(BankableMIDIInputAddressable<N> *bankable) {
    inputBankables.append(bankable);
}

template <setting_t N>
void Bank<N>::remove(BankableMIDIInputAddressable<N> *bankable) {
    inputBankables.remove(bankable);
}

template <setting_t N>
void Bank<N>::select(setting_t bankSetting) {
    bankSetting = this->validateSetting(bankSetting);
    OutputBank::select(bankSetting);
    for (BankableMIDIInputAddressable<N> &e : inputBankables)
        e.onBankSettingChange();
}