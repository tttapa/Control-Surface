#pragma once

#include <Helpers/Debug.hpp>
#include <Helpers/LinkedList.h>
#include <Selectors/Selectable.hpp>
#include <stddef.h>
#include <stdint.h>

template <setting_t N>
class BankableMIDIInputAddressable;

class BankableMIDIOutputAddressable;
class BankableMIDIOutput;

class OutputBank {
  public:
    OutputBank(uint8_t tracksPerBank) : tracksPerBank(tracksPerBank) {
        if (tracksPerBank == 0) {
            DEBUGFN(F("Error: A Bank must have a non-zero number of tracks."));
            FATAL_ERROR();
        }
    }
    void select(setting_t setting) { bankSetting = setting; }
    virtual uint8_t getSelection() const { return bankSetting; }
    uint8_t getTracksPerBank() const { return tracksPerBank; }

    const uint8_t tracksPerBank;
    uint8_t bankSetting = 0;
};

/**
 * @brief   A class that groups MIDI_Element%s and allows the user to change
 *          the MIDI addresses (Controller number or Note number) or the MIDI
 *          channels of these elements.
 *
 * Consider two controls: `volume_A` and `volume_B`:
 * `volume_A` has controller 0x07 on MIDI channel 1, and
 * `volume_B` has controller 0x07 on MIDI channel 2.
 * Let's add them to a bank with 2 tracks per bank.
 * @todo    Example
 * @code{.cpp}
 * AnalogCC volume_A(A0, MIDI_CC::Channel_Volume, 1);
 * AnalogCC volume_B(A1, MIDI_CC::Channel_Volume, 2);
 *
 * Bank bank(2);
 *
 * bank.add(volume_A, Bank::CHANGE_CHANNEL);
 * bank.add(volume_B, Bank::CHANGE_CHANNEL);
 * @endcode
 * By default, the bank setting is 0, so it has no impact:
 * `volume_A` still sends MIDI messages on channel 1, and
 * `volume_B` still sends MIDI messages on channel 2.
 * However, when the bank setting is changed, this is
 * no longer the case:
 * @code{.cpp}
 * bank.select(1);
 * @endcode
 * Now that the bank setting is set to one, the number of
 * tracks per bank is added to the channel of each element
 * in the bank:
 * `volume_A` now sends MIDI messages on channel 3, and
 * `volume_B` now sends MIDI messages on channel 4.
 *
 * When the bank setting is set to 2, `volume_A` will use
 * channel 5, and `volume_B` channel 6, etc.
 *
 * When using the bank type Bank::CHANGE_ADDRESS, the same
 * thing happens, but the address changes and the channel
 * stays the same.
 *
 * To change the bank setting, a BankSelector can be used.
 *
 * Apart from changing the address or channel of an element,
 * banks can also be used to apply a single map function
 * to many Potentiometer (CCPotentiometer or PBPotentiometer)
 * elements at the same time.
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
    Bank(uint8_t tracksPerBank = 1, uint8_t initialSelection = 0)
        : Selectable<N>(initialSelection), OutputBank(tracksPerBank) {}

    /**
     * @brief   Set the Bank Setting
     *
     * @param   bankSetting
     *          The new Bank Setting.
     */
    void select(uint8_t bankSetting) override;

    void remove(BankableMIDIInputAddressable<N> *bankable);

  private:
    /**
     * @brief   Add a Bankable to the bank.
     *
     * @param   bankable
     *          The Bankable to be added.
     */
    void add(BankableMIDIInputAddressable<N> &bankable);

    BankableMIDIInputAddressable<N> *first = nullptr;
    BankableMIDIInputAddressable<N> *last = nullptr;

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};

#include "Bank.ipp"