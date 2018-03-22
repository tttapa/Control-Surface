#ifndef SELECTOR_OUT_H
#define SELECTOR_OUT_H

// #include <Arduino.h>
#include "../Settings/Settings.h"
#include "../Selectors/Selector.h"
#include "../Control_Surface/Control_Surface_Class.h"

class SelectorPC : public Selector
{
  public:
    template <class... Args>
    SelectorPC(std::initializer_list<uint8_t> addresses, Args... args) : Selector(args...) {
        init(addresses);
    }
    // initializer_lists are not supported with variadic templates, so overload them manually
    SelectorPC(std::initializer_list<uint8_t> addresses, std::initializer_list<pin_t> switchPins, std::initializer_list<pin_t> ledPins) : Selector(switchPins, ledPins), nb_addresses(addresses.size()) {
        static_assert(addresses.size() == ledPins.size(), "Error");
    }
    // SelectorPC(Bank &bank, std::initializer_list<pin_t> switchPins) : Selector(switchPins), bank(bank) {}
    // SelectorPC(Bank &bank, std::initializer_list<pin_t> switchPins, pin_t nb_settings) : Selector(switchPins, nb_settings), bank(bank) {}

    ~SelectorPC() {
        delete[] addressStorage;
    }

  private:
    void refreshImpl(uint8_t newSetting) {
        Control_Surface.MIDI()->send(PITCH_BEND,
                                     channel,
                                     addressStorage[newSetting % nb_addresses]);
    }

    void init(std::initializer_list<uint8_t> addresses) {
        addressStorage = new uint8_t[addresses.size()];
        memcpy(addressStorage, addresses.begin(), addresses.size());
    }

    uint8_t *addressStorage = nullptr;
    const size_t nb_addresses;
    const uint8_t channel = 0;
};

#endif // SELECTOR_OUT_H
