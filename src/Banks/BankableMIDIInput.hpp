#pragma once

#include "Bank.hpp"
#include "BankConfig.hpp"
#include <Def/MIDICNChannelAddress.hpp>
#include <AH/Debug/Debug.hpp>
#include <AH/Containers/LinkedList.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A base class for all MIDIInputElement%s that can be banked.
 * 
 * @note    These elements must be updated when the bank setting is changed, so 
 *          they are added to a linked list of the bank.
 * 
 * @tparam  N
 *          The number of banks.
 */
template <setting_t N>
class BankableMIDIInput : public DoublyLinkable<BankableMIDIInput<N>> {
    friend class Bank<N>;

  protected:
    /**
     * @brief   Create a new BankableMIDIInput object, and add it to the bank.
     * 
     * @param   bank
     *          The bank to add this element to.
     * @param   type
     *          What address type to change (address, channel or cable number).
     */
    BankableMIDIInput(Bank<N> &bank, BankType type) : bank(bank), type(type) {
        bank.add(this);
    }

    /**
     * @brief   Create a new BankableMIDIInput object, and add it to the bank.
     * 
     * @param   config
     *          The bank and address type to change.
     * 
     * @see     BankableMIDIInput::BankableMIDIInput(Bank<N> &, BankType)
     */
    BankableMIDIInput(const BankConfig<N> &config)
        : BankableMIDIInput<N>(config.bank, config.type) {}

  public:
    /** 
     * @brief   Destructor: remove element from the bank.
     */
    virtual ~BankableMIDIInput() { bank.remove(this); }

    /** 
     * @brief   Get the current bank setting.
     * 
     * @see     Bank<N>::getSelection()
     */
    setting_t getSelection() const { return bank.getSelection(); }

    /**
     * @brief   Calculate the bank setting of a given MIDI address, relative to
     *          a base address.
     * 
     * @param   target 
     *          The MIDI address to calculate the bank setting of.
     * @param   base
     *          The base address to compare it to (the address of bank setting 
     *          0).
     */
    uint8_t getBankIndex(const MIDICNChannelAddress &target,
                         const MIDICNChannelAddress &base) const {
        switch (type) {
            case CHANGE_ADDRESS:
                return (target.getAddress() - base.getAddress()) /
                       bank.getTracksPerBank();
            case CHANGE_CHANNEL:
                return (target.getRawChannel() - base.getRawChannel()) /
                       bank.getTracksPerBank();
            case CHANGE_CABLENB:
                return (target.getCableNumber() - base.getCableNumber()) /
                       bank.getTracksPerBank();
            default: return 0;
        }
    }

  protected:
    /**
     * @brief   Check if the given address is part of the bank relative to the
     *          base address.
     * 
     * Consider the following example:  
     * A Bank with 4 tracks per bank (T), 2 bank settings (N), 
     * and a base address of 3.
     * 
     * ```
     * 0   1   2   3   4   5   6   7   8   9  10  11  12  ...
     * F   F   F   T   F   F   F   T   F   F   F   F   F  ...
     * ```
     * 
     * Addresses before the base adddress are not matched (0, 1, 2).  
     * Addresses after N * T are not matched (8, 9, 10, 11, 12).  
     * Addresses with a distance to the base address that is not a multiple of N
     * are not matched (4, 5, 6).
     * 
     * @param   toMatch 
     *          The address to check.
     * @param   base
     *          The base address (the address of bank setting 0).
     * 
     * @note    Equivalent to `matchBankableInRange(toMatch, base, 1)`.
     */
    bool matchBankable(uint8_t toMatch, uint8_t base) const {
        uint8_t diff = toMatch - base;
        return toMatch >= base && diff < N * bank.getTracksPerBank() &&
               diff % bank.getTracksPerBank() == 0;
    }

    /**
     * @brief   Check if the given address is part of the bank relative to the
     *          base address.
     * 
     * @todo    This is very hard to explain without a specific example ...
     * 
     * @param   toMatch 
     *          The address to check.
     * @param   base
     *          The base address (the address of bank setting 0).
     * @param   length
     *          The length of the range.
     */
    bool matchBankableInRange(uint8_t toMatch, uint8_t base,
                              uint8_t length) const {
        uint8_t diff = toMatch - base;
        return toMatch >= base && diff < N * bank.getTracksPerBank() &&
               diff % bank.getTracksPerBank() < length;
    }

    /**
     * @brief   If matchBankableAddressInRange returned true, get the index of
     *          the message in the range.
     */
    uint8_t getRangeIndex(MIDICNChannelAddress target,
                          MIDICNChannelAddress base) const {
        uint8_t diff = target.getAddress() - base.getAddress();
        if (type == CHANGE_ADDRESS)
            diff %= bank.getTracksPerBank();
        return diff;
    }

    /**
     * @brief   Check whether a given address is within a range of given length
     *          starting from the given base address.
     * 
     * @param   toMatch
     *          The address to check
     * @param   base
     *          The base address, start of the range.
     * @param   length
     *          The length of the range.
     */
    static bool inRange(uint8_t toMatch, uint8_t base, uint8_t length) {
        return (base <= toMatch) && (toMatch - base < length);
    }

    /**
     * @brief   Check whether a given address is part of the bank relative to
     *          the base address.
     * 
     * @param   toMatch 
     *          The address to check.
     * @param   base
     *          The base address (the address of bank setting 0).
     */
    bool matchBankable(const MIDICNChannelAddress &toMatch,
                       const MIDICNChannelAddress &base) const {
        if (!toMatch.isValid() || !base.isValid())
            return false;
        switch (type) {
            case CHANGE_ADDRESS: {
                return toMatch.getChannel() == base.getChannel() &&
                       toMatch.getCableNumber() == base.getCableNumber() &&
                       matchBankable(toMatch.getAddress(), base.getAddress());
            }
            case CHANGE_CHANNEL: {
                return toMatch.getAddress() == base.getAddress() &&
                       toMatch.getCableNumber() == base.getCableNumber() &&
                       matchBankable(toMatch.getRawChannel(),
                                     base.getRawChannel());
            }
            case CHANGE_CABLENB: {
                return toMatch.getAddress() == base.getAddress() &&
                       toMatch.getChannel() == base.getChannel() &&
                       matchBankable(toMatch.getCableNumber(),
                                     base.getCableNumber());
            }
            default: return false;
        }
    }

    /**
     * @brief   Check whether a given address is part of the bank relative to
     *          the base address and within a range with a given length.
     * 
     * @param   toMatch 
     *          The address to check.
     * @param   base
     *          The base address (the address of bank setting 0).
     * @param   length
     *          The length of the range.
     */
    bool matchBankableAddressInRange(const MIDICNChannelAddress &toMatch,
                                     const MIDICNChannelAddress &base,
                                     uint8_t length) const {
        if (!toMatch.isValid() || !base.isValid())
            return false;
        switch (type) {
            case CHANGE_ADDRESS: {
                return toMatch.getChannel() == base.getChannel() &&
                       toMatch.getCableNumber() == base.getCableNumber() &&
                       matchBankableInRange(toMatch.getAddress(),
                                            base.getAddress(), length);
            }
            case CHANGE_CHANNEL: {
                return inRange(toMatch.getAddress(), base.getAddress(),
                               length) &&
                       toMatch.getCableNumber() == base.getCableNumber() &&
                       matchBankable(toMatch.getChannel(), base.getChannel());
            }
            case CHANGE_CABLENB: {
                return inRange(toMatch.getAddress(), base.getAddress(),
                               length) &&
                       toMatch.getChannel() == base.getChannel() &&
                       matchBankable(toMatch.getCableNumber(),
                                     base.getCableNumber());
            }
            default: return false;
        }
    }

  private:
    Bank<N> &bank;
    const BankType type;

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

END_CS_NAMESPACE