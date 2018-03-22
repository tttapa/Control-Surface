#ifndef SELECTOR_OUT_H
#define SELECTOR_OUT_H

// #include <Arduino.h>
#include "../Settings/Settings.h"
#include "../Selectors/Selector.h"
#include "../Control_Surface/Control_Surface_Class.h"
#include "../Helpers/Copy.hpp"

class SelectorPC : public Selector
{
  public:
    template <class A, size_t N, class... Args>
    SelectorPC(const A (&addresses)[N], Args... args) 
        : Selector(args...),
          nb_addresses(N)
    {
        this->addresses = new uint8_t[nb_addresses];
        copy(this->addresses, addresses);
    }
    // initializer_lists are not supported with variadic templates, so overload them manually
    template <class A, class T, size_t N>
    SelectorPC(const A (&addresses)[N], const T (&switchPins)[N]) 
        : Selector((const pin_t (&)[N])switchPins), // Multiple buttons, no LEDs
          nb_addresses(N)
    {
        this->addresses = new uint8_t[nb_addresses];
        copy(this->addresses, addresses);
    }
    template <class A, class T, class S, size_t N, size_t M>
    SelectorPC(const A (&addresses)[M], const T (&switchPins)[N], const S (&ledPins)[M]) 
        : Selector((const pin_t (&)[N])switchPins, (const pin_t (&)[M])ledPins), // One (+1), two (+1, -1) or multiple buttons, multiple LEDs
          nb_addresses(M)
    {
        this->addresses = new uint8_t[nb_addresses];
        copy(this->addresses, addresses);
    }
    template <class A, class T, class S, size_t N, size_t M>
    SelectorPC(const A (&addresses)[M], const T (&switchPins)[N], S nb_settings) 
        : Selector((const pin_t (&)[N])switchPins, (size_t)nb_settings), // One (+1) or two (+1, -1) buttons, no LEDs
          nb_addresses(M)
    {
        this->addresses = new uint8_t[nb_addresses];
        copy(this->addresses, addresses);
    }
    ~SelectorPC()
    {
        delete[] addresses;
    }

  private:
    void refreshImpl(uint8_t newSetting)
    {
        Control_Surface.MIDI()->send(PROGRAM_CHANGE,
                                     channel,
                                     addresses[newSetting % nb_addresses]);
    }

    uint8_t *addresses = nullptr;
    const size_t nb_addresses;
    const uint8_t channel = 0;
};

#endif // SELECTOR_OUT_H
