#include "Bank.h"
#include "BankableMIDIInputAddressable.hpp"
#include "BankableMIDIOutput.hpp"
#include "BankableMIDIOutputAddressable.hpp"
#include <Selectors/Selector.hpp>

template <setting_t N>
void Bank<N>::add(BankableMIDIInputAddressable<N> &bankable) {
    LinkedList::append(&bankable, first, last);
}

template <setting_t N>
void Bank<N>::select(setting_t bankSetting) {
    bankSetting = this->validateSetting(bankSetting);
    OutputBank::select(bankSetting);
    first->onBankSettingChangeAll();
}

template <setting_t N>
void Bank<N>::remove(BankableMIDIInputAddressable<N> *bankable) {
    LinkedList::remove(bankable, first, last);
}