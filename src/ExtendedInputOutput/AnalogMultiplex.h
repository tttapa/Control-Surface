#ifndef ANALOGMULTIPLEX_H_
#define ANALOGMULTIPLEX_H_

#include "ExtendedIOElement.h"
#include <stdlib.h>
#include "../Helpers/Copy.hpp"

class AnalogMultiplex : public ExtendedIOElement
{
public:
  template <class P, size_t N>
  AnalogMultiplex(pin_t analogPin, const P (&addressPins)[N])
      : ExtendedIOElement(1 << N), analogPin(analogPin), nb_addressPins(N), nb_addresses(1 << nb_addressPins)
  {
    this->addressPins = new pin_t[N];
    copy(this->addressPins, addressPins);
    begin();
  }
  ~AnalogMultiplex()
  {
    free(addressPins);
  }

  int digitalRead(pin_t pin);
  analog_t analogRead(pin_t pin);
  void pinMode(pin_t, uint8_t mode);
  void begin();

private:
  const pin_t analogPin;
  pin_t *addressPins = nullptr;
  const size_t nb_addressPins, nb_addresses;

  uint8_t pinToMuxAddress(pin_t pin);
  void setMuxAddress(uint8_t address);
};

#endif // ANALOGMULTIPLEX_H_