#ifndef SELECTOR_OUT_H
#define SELECTOR_OUT_H

// #include <Arduino.h>
#include "../Settings/SettingsWrapper.h"
#include "./Selector.h"
#include "../Control_Surface/Control_Surface_Class.h"
#include "../Helpers/Copy.hpp"

class SelectorPC : public Selector
{
  public:
    template <class A, size_t N, class... Args>
    SelectorPC(const A (&addresses)[N], uint8_t channel, Args... args) 
        : Selector(args...), channel(channel)
    {
        this->addresses = new uint8_t[nb_settings];
        copy(this->addresses, addresses);
    }
    // initializer_lists are not supported with variadic templates, so overload them manually
    template <class A, class T, size_t N>
    SelectorPC(const A (&addresses)[N], uint8_t channel, const T (&switchPins)[N]) 
        : Selector((const pin_t (&)[N])switchPins), channel(channel) // Multiple buttons, no LEDs
    {
        this->addresses = new uint8_t[nb_settings];
        copy(this->addresses, addresses);
    }
    template <class A, class T, class S, size_t N, size_t M>
    SelectorPC(const A (&addresses)[M], uint8_t channel, const T (&switchPins)[N], const S (&ledPins)[M]) 
        : Selector((const pin_t (&)[N])switchPins, (const pin_t (&)[M])ledPins), channel(channel) // One (+1), two (+1, -1) or multiple buttons, multiple LEDs
    {
        this->addresses = new uint8_t[nb_settings];
        copy(this->addresses, addresses);
    }
    template <class A, class T, size_t N, size_t M>
    SelectorPC(const A (&addresses)[M], uint8_t channel, const T (&switchPins)[N]) 
        : Selector((const pin_t (&)[N])switchPins, M), channel(channel) // One (+1) or two (+1, -1) buttons, no LEDs
    {
        this->addresses = new uint8_t[nb_settings];
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
                                     addresses[newSetting]);
    }

    uint8_t *addresses = nullptr;
    const uint8_t channel;
};

#endif // SELECTOR_OUT_H
