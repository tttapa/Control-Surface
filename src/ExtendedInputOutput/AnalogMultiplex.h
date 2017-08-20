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
  template <size_t N>
  AnalogMultiplex(pin_t analogPin, const pin_t (&addressPins)[N])
      : analogPin(analogPin), addressPins(addressPins), nb_addressPins(N), nb_addresses(1 << nb_addressPins),
        ExtendedIOElement(1 << N)
  {
    ;
  }
  AnalogMultiplex(pin_t analogPin, std::initializer_list<pin_t> addressPins)
      : analogPin(analogPin), nb_addressPins(addressPins.size()), nb_addresses(1 << addressPins.size()),
        ExtendedIOElement(1 << addressPins.size())
  {
    addressPinsStorage = (pin_t *)malloc(sizeof(pin_t) * addressPins.size());
    memcpy(addressPinsStorage, addressPins.begin(), sizeof(pin_t) * addressPins.size());
    this->addressPins = addressPinsStorage;
  }
  ~AnalogMultiplex()
  {
    free(addressPinsStorage);
  }

  int digitalRead(pin_t pin);
  analog_t analogRead(pin_t pin);
  void begin();

private:
  const pin_t analogPin;
  const pin_t *addressPins;
  pin_t *addressPinsStorage = nullptr;
  const size_t nb_addressPins, nb_addresses;

  uint8_t pinToMuxAddress(pin_t pin);
  void setMuxAddress(uint8_t address);
};

#endif // ANALOGMULTIPLEX_H_