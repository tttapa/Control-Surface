#pragma once

#include <Helpers/Debug.hpp>
#include <Helpers/LinkedList.h>
#include <stddef.h>
#include <stdint.h>

class BankableMIDIInputAddressable;
class BankableMIDIOutputAddressable;
class BankableMIDIOutput;

/**
 * @brief   A class that groups MIDI_Element%s and allows the user to change
 *          the MIDI addresses (Controller number or Note number) or the MIDI
 *          channels of these elements.
 *
 * Consider two controls: `volume_A` and `volume_B`:
 * `volume_A` has controller 0x07 on MIDI channel 1, and
 * `volume_B` has controller 0x07 on MIDI channel 2.
 * Let's add them to a bank with 2 tracks per bank.
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
 * bank.setBankSetting(1);
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
class Bank {
  public:
    /**
     * @brief   Construct a new Bank object.
     *
     * @param   tracksPerBank
     *          The number of tracks (i.e. addresses or channels) that
     *          are skipped when the bank setting changes.
     */
    Bank(uint8_t tracksPerBank = 1);

    enum bankType {
        /**
         * @brief   Change the offset of the channel number of the Bankable.
         */
        CHANGE_CHANNEL,
        /**
         * @brief   Change the offset of the address (i.e. Controller number or
         *          Note number) of the MIDI_Element.
         */
        CHANGE_ADDRESS
    };

    /**
     * @brief   Add a Bankable to the bank.
     *
     * @param   bankable
     *          A pointer to the Bankable to be added.
     * @param   type
     *          Either change the MIDI channel or the address.
     */
    void add(BankableMIDIInputAddressable *bankable,
             bankType type = CHANGE_ADDRESS);

    /**
     * @brief   Add a Bankable to the bank.
     *
     * @param   bankable
     *          The Bankable to be added.
     * @param   type
     *          Either change the MIDI channel or the address.
     */
    void add(BankableMIDIInputAddressable &bankable,
             bankType type = CHANGE_ADDRESS);

    /**
     * @brief   Add an array of Bankable%s to the bank.
     *
     * @tparam  T
     *          The type of the Bankable%s.
     * @tparam  N
     *          The number of Bankable%s in the array.
     *
     * @param   bankables
     *          An array of Bankable%s to be added.
     * @param   type
     *          Either change the MIDI channel or the address of the bankables.
     *
     * @todo    Do I need the T template paramter?
     */
    template <class T, size_t N>
    void add(T (&bankables)[N], bankType type = CHANGE_ADDRESS) {
        for (T &bankable : bankables)
            add(bankable, type);
    }

    /**
     * @todo    Documentation.
     */
    void add(BankableMIDIOutputAddressable &bankable,
             bankType type = CHANGE_ADDRESS);

    /**
     * @brief   Add a Bankable to the bank.
     *
     * @param   bankable
     *          The Bankable to be added.
     */
    void add(BankableMIDIOutput &bankable);

    /**
     * @brief   Set the Bank Setting
     *
     * @param   bankSetting
     *          The new Bank Setting.
     */
    void setBankSetting(uint8_t bankSetting);

    uint8_t getBankSetting() const;

    uint8_t getTracksPerBank() const;

    void remove(BankableMIDIInputAddressable *bankable);

  private:
    const uint8_t tracksPerBank;
    uint8_t bankSetting = 0;
    BankableMIDIInputAddressable *first = nullptr;
    BankableMIDIInputAddressable *last = nullptr;

    template <class Node>
    friend void LinkedList::append(Node *, Node *&, Node *&);
    template <class Node>
    friend void LinkedList::remove(Node *, Node *&, Node *&);
};