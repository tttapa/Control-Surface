#ifndef ANALOGMULTIPLEX_H_
#define ANALOGMULTIPLEX_H_

#include "Arduino.h"
#include "ExtendedIOElement.h"
#ifdef __AVR__
#include "../initializer_list.h"
#else
#include <initializer_list>
#endif

class AnalogMultiplex : public ExtendedIOElement
{
  public:
    template <size_t M, size_t N>
    AnalogMultiplex(pin_t (&analogPins)[M], pin_t (&addressPins)[N])
        : analogPins(analogPins), addressPins(addressPins), nb_analogPins(M), nb_addressPins(N), nb_addresses(1 << nb_addressPins),
          ExtendedIOElement(nb_analogPins * (1 << nb_addressPins))
    {
        ;
    }
    template <size_t N>
    AnalogMultiplex(pin_t &analogPin, pin_t (&addressPins)[N])
        : analogPins(&analogPin), addressPins(addressPins), nb_analogPins(1), nb_addressPins(N), nb_addresses(1 << nb_addressPins),
          ExtendedIOElement(nb_analogPins * (1 << nb_addressPins))
    {
        ;
    }
    AnalogMultiplex(std::initializer_list<pin_t> analogPins, std::initializer_list<pin_t> addressPins)
        : nb_analogPins(analogPins.size()), nb_addressPins(addressPins.size()), nb_addresses(1 << nb_addressPins),
          ExtendedIOElement(nb_analogPins * (1 << nb_addressPins))
    {
        analogPinsStorage = (pin_t *)malloc(sizeof(pin_t) * analogPins.size());
        memcpy(analogPinsStorage, analogPins.begin(), sizeof(pin_t) * analogPins.size());
        this->analogPins = analogPinsStorage;
        addressPinsStorage = (pin_t *)malloc(sizeof(pin_t) * addressPins.size());
        memcpy(addressPinsStorage, addressPins.begin(), sizeof(pin_t) * addressPins.size());
        this->addressPins = addressPinsStorage;
    }
    ~AnalogMultiplex()
    {
        free(analogPinsStorage);
        free(addressPinsStorage);
    }

    int digitalRead(pin_t pin);
    analog_t analogRead(pin_t pin);
    void begin();

  private:
    const pin_t *analogPins, *addressPins;
    pin_t *analogPinsStorage = nullptr, *addressPinsStorage = nullptr;
    const size_t nb_analogPins, nb_addressPins, nb_addresses;

    pin_t pinToAnalogPin(pin_t pin)
    {
        return pin / nb_addresses;
    }
    uint8_t pinToMuxAddress(pin_t pin) {
        return pin % nb_addresses;
    }
    const int8_t INVALID_PIN = -1;
};

#endif // ANALOGMULTIPLEX_H_