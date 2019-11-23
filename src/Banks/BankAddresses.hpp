#pragma once

#include "BankableMIDIOutput.hpp"

BEGIN_CS_NAMESPACE

namespace Bankable {

class SingleAddress : public BankableMIDIOutput {
  public:
    SingleAddress(OutputBankConfig config, MIDICNChannelAddress address)
        : BankableMIDIOutput{config}, address{address} {}

    MIDICNChannelAddress getBaseAddress() const { return address; }

    MIDICNChannelAddress getActiveAddress() const {
        return getBaseAddress() + getAddressOffset();
    }

  private:
    MIDICNChannelAddress address;
};

class TwoSingleAddresses : public BankableMIDIOutput {
  public:
    TwoSingleAddresses(OutputBankConfig config,
                       const Array<MIDICNChannelAddress, 2> &addresses)
        : BankableMIDIOutput{config}, addresses(addresses) {}

    MIDICNChannelAddress getBaseAddress(uint8_t i) const {
        return addresses[i];
    }

    MIDICNChannelAddress getActiveAddress(uint8_t i) const {
        return getBaseAddress(i) + getAddressOffset();
    }

  private:
    Array<MIDICNChannelAddress, 2> addresses;
};

template <uint8_t nb_rows, uint8_t nb_cols>
class MatrixAddress : public BankableMIDIOutput {
  public:
    MatrixAddress(OutputBankConfig config,
                  const AddressMatrix<nb_rows, nb_cols> &addresses,
                  MIDICNChannel channelCN)
        : BankableMIDIOutput{config}, addresses{addresses}, channelCN{
                                                                channelCN} {}

    uint8_t getBaseAddress(uint8_t row, uint8_t col) const {
        return addresses[row][col];
    }

    MIDICNChannelAddress getActiveAddress(uint8_t row, uint8_t col) const {
        MIDICNChannelAddress address = {getBaseAddress(row, col), channelCN};
        return address + getAddressOffset();
    }

  private:
    AddressMatrix<nb_rows, nb_cols> addresses;
    MIDICNChannel channelCN;
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
                  const Array<MIDICNChannelAddress, NumBanks> &addresses)
        : ManyAddressesMIDIOutput{bank}, addresses{addresses} {}

    MIDICNChannelAddress getActiveAddress() const {
        return addresses[getSelection()];
    }

  private:
    Array<MIDICNChannelAddress, NumBanks> addresses;
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
        const Array2D<MIDICNChannelAddress, 2, NumBanks> &addresses)
        : BankableMIDIOutput{config}, addresses{addresses} {}

    MIDICNChannelAddress getActiveAddress(uint8_t i) const {
        return addresses[i][getSelection()];
    }

  private:
    Array2D<MIDICNChannelAddress, 2, NumBanks> addresses;
};

template <uint8_t NumBanks, uint8_t nb_rows, uint8_t nb_cols>
class ManyMatrixAddresses : public BankableMIDIOutput {
  public:
    ManyMatrixAddresses(
        OutputBankConfig config,
        const Array<AddressMatrix<nb_rows, nb_cols>, NumBanks> &addresses,
        const Array<MIDICNChannel, NumBanks> &channelCNs)
        : BankableMIDIOutput{config}, addresses{addresses}, channelCNs{
                                                                channelCNs} {}

    MIDICNChannelAddress getActiveAddress(uint8_t row, uint8_t col) const {
        return {addresses[getSelection()][row][col],
                channelCNs[getSelection()]};
    }

  private:
    Array<AddressMatrix<nb_rows, nb_cols>, NumBanks> addresses;
    Array<MIDICNChannel, NumBanks> channelCNs;
};

} // namespace ManyAddresses
} // namespace Bankable

END_CS_NAMESPACE