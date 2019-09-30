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

} // namespace ManyAddresses
} // namespace Bankable

END_CS_NAMESPACE