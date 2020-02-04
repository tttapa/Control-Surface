#pragma once

#include "BankableMIDIOutput.hpp"

BEGIN_CS_NAMESPACE

namespace Bankable {

class SingleAddress : public BankableMIDIOutput {
  public:
    SingleAddress(OutputBankConfig config, MIDIAddress address)
        : BankableMIDIOutput{config}, address{address} {}

    MIDIAddress getBaseAddress() const { return address; }

    MIDIAddress getActiveAddress() const {
        return getBaseAddress() + getAddressOffset();
    }

  private:
    MIDIAddress address;
};

class TwoSingleAddresses : public BankableMIDIOutput {
  public:
    TwoSingleAddresses(OutputBankConfig config,
                       const Array<MIDIAddress, 2> &addresses)
        : BankableMIDIOutput{config}, addresses(addresses) {}

    MIDIAddress getBaseAddress(uint8_t i) const {
        return addresses[i];
    }

    MIDIAddress getActiveAddress(uint8_t i) const {
        return getBaseAddress(i) + getAddressOffset();
    }

  private:
    Array<MIDIAddress, 2> addresses;
};

template <uint8_t nb_rows, uint8_t nb_cols>
class MatrixAddress : public BankableMIDIOutput {
  public:
    MatrixAddress(OutputBankConfig config,
                  const AddressMatrix<nb_rows, nb_cols> &addresses,
                  MIDIChannelCN channelCN)
        : BankableMIDIOutput{config}, addresses{addresses}, channelCN{
                                                                channelCN} {}

    uint8_t getBaseAddress(uint8_t row, uint8_t col) const {
        return addresses[row][col];
    }

    MIDIAddress getActiveAddress(uint8_t row, uint8_t col) const {
        MIDIAddress address = {getBaseAddress(row, col), channelCN};
        return address + getAddressOffset();
    }

  private:
    AddressMatrix<nb_rows, nb_cols> addresses;
    MIDIChannelCN channelCN;
};

namespace ManyAddresses {

/**
 * @tparam  NumBanks
 *          The number of bank settings the bank has.
 */
template <uint8_t NumBanks>
class ManyAddresses : public ManyAddressesMIDIOutput {
  public:
    /**
     * @brief   Constructor.
     * 
     * @param   bank
     *          The bank to add this element to.
     * @param   addresses
     *          The list of alternative addresses.
     */
    ManyAddresses(const Bank<NumBanks> &bank,
                  const Array<MIDIAddress, NumBanks> &addresses)
        : ManyAddressesMIDIOutput{bank}, addresses{addresses} {}

    MIDIAddress getActiveAddress() const {
        return addresses[getSelection()];
    }

  private:
    Array<MIDIAddress, NumBanks> addresses;
};

/**
 * @tparam  NumBanks
 *          The number of bank settings the bank has.
 */
template <uint8_t NumBanks>
class TwoManyAddresses : public BankableMIDIOutput {
  public:
    TwoManyAddresses(
        OutputBankConfig config,
        const Array2D<MIDIAddress, 2, NumBanks> &addresses)
        : BankableMIDIOutput{config}, addresses{addresses} {}

    MIDIAddress getActiveAddress(uint8_t i) const {
        return addresses[i][getSelection()];
    }

  private:
    Array2D<MIDIAddress, 2, NumBanks> addresses;
};

template <uint8_t NumBanks, uint8_t nb_rows, uint8_t nb_cols>
class ManyMatrixAddresses : public BankableMIDIOutput {
  public:
    ManyMatrixAddresses(
        OutputBankConfig config,
        const Array<AddressMatrix<nb_rows, nb_cols>, NumBanks> &addresses,
        const Array<MIDIChannelCN, NumBanks> &channelCNs)
        : BankableMIDIOutput{config}, addresses{addresses}, channelCNs{
                                                                channelCNs} {}

    MIDIAddress getActiveAddress(uint8_t row, uint8_t col) const {
        return {addresses[getSelection()][row][col],
                channelCNs[getSelection()]};
    }

  private:
    Array<AddressMatrix<nb_rows, nb_cols>, NumBanks> addresses;
    Array<MIDIChannelCN, NumBanks> channelCNs;
};

} // namespace ManyAddresses
} // namespace Bankable

END_CS_NAMESPACE