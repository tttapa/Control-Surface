#pragma once

#include "Bank.h"

class BankConfigAddressable {
  public:
    BankConfigAddressable(Bank &bank,
                          Bank::bankType type = Bank::CHANGE_ADDRESS)
        : bank(bank), type(type) {}
    Bank &bank;
    const Bank::bankType type;
};