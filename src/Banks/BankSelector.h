#ifndef BANKSELECTOR_H_
#define BANKSELECTOR_H_

#include "./Bank.h"
#include "../Selectors/Selector.h"

class BankSelector : public Selector
{  
public:
  template<class...Args>
  BankSelector(Bank &bank, Args...args) : Selector(args...), bank(bank) {}
  // initializer_lists are not supported with variadic templates, so overload them manually
  BankSelector(Bank &bank, std::initializer_list<pin_t> switchPins, std::initializer_list<pin_t> ledPins) : Selector(switchPins, ledPins), bank(bank) {}
  BankSelector(Bank &bank, std::initializer_list<pin_t> switchPins) : Selector(switchPins), bank(bank) {}
  BankSelector(Bank &bank, std::initializer_list<pin_t> switchPins, pin_t nb_settings) : Selector(switchPins, nb_settings), bank(bank) {}

private: 
  Bank &bank;
  void refreshImpl(uint8_t newSetting) {
    bank.setBankSetting(newSetting);
  }
};

#endif // BANKSELECTOR_H_