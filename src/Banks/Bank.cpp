#include "Bank.h"
#include "BankableMIDIInputAddressable.hpp"
#include "BankableMIDIOutputAddressable.hpp"
#include "BankableMIDIOutput.hpp"

Bank::Bank(uint8_t tracksPerBank) : tracksPerBank(tracksPerBank) {
    if (tracksPerBank == 0) {
        DEBUGFN(F("Error: A Bank must have a non-zero number of tracks."));
        FATAL_ERROR();
    }
}

void Bank::add(BankableMIDIInputAddressable &bankable) {
    LinkedList::append(&bankable, first, last);
}

void Bank::setBankSetting(uint8_t bankSetting) {
    this->bankSetting = bankSetting;
    first->onBankSettingChangeAll();
}

uint8_t Bank::getBankSetting() const { return bankSetting; }

uint8_t Bank::getTracksPerBank() const { return tracksPerBank; }

void Bank::remove(BankableMIDIInputAddressable *bankable) {
    LinkedList::remove(bankable, first, last);
}
