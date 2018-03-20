#include "AnalogMultiplex.h"
#include "ExtendedInputOutput.h"

using namespace ExtIO;

int AnalogMultiplex::digitalRead(pin_t pin)
{
    setMuxAddress(pinToMuxAddress(pin));
    return digitalRead(analogPin);
}
analog_t AnalogMultiplex::analogRead(pin_t pin)
{
    setMuxAddress(pinToMuxAddress(pin));
    analogRead(analogPin);
    return analogRead(analogPin);
}
void AnalogMultiplex::begin()
{
    pinMode(analogPin, INPUT);
    for (uint8_t i = 0; i < nb_addressPins; i++)
        pinMode(addressPins[i], OUTPUT);
}

uint8_t AnalogMultiplex::pinToMuxAddress(pin_t pin)
{
    return pin & (nb_addresses - 1);
}
void AnalogMultiplex::setMuxAddress(uint8_t address)
{
    for (uint8_t i = 0; i < nb_addressPins; i++)
        digitalWrite(addressPins[i], address & (1 << i));
}
