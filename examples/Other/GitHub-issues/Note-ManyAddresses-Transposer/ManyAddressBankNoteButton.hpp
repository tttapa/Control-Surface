#include <Control_Surface.h>

BEGIN_CS_NAMESPACE

namespace Bankable {
namespace ManyAddresses {

template <uint8_t NumAddr>
class ManyAddressBank {
 public:
  ManyAddressBank(const ManyAddresses<NumAddr> &manyaddresses,
                  BaseOutputBankConfig bank)
    : manyaddresses(manyaddresses), bank(bank) {}

  void lock() { manyaddresses.lock(), bank.lock(); }
  void unlock() { manyaddresses.unlock(), bank.unlock(); }

  MIDIAddress getActiveAddress() const {
    return manyaddresses.getActiveAddress() + bank.getAddressOffset();
  }

 private:
  ManyAddresses<NumAddr> manyaddresses;
  OutputBankableMIDIAddress bank;
};

template <uint8_t NumAddr>
class ManyAddressBankNoteButton
  : public MIDIButton<ManyAddressBank<NumAddr>, DigitalNoteSender> {
 public:
  ManyAddressBankNoteButton(const ManyAddresses<NumAddr> &manyaddresses,
                            OutputBankConfig<> bank, pin_t pin,
                            const DigitalNoteSender &sender {})
    : MIDIButton<ManyAddressBank<NumAddr>, DigitalNoteSender>(
        {manyaddresses, bank}, pin, sender) {}
};

} // namespace ManyAddresses
} // namespace Bankable

END_CS_NAMESPACE