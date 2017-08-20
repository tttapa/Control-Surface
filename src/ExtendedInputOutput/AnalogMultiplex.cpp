#include "AnalogMultiplex.h"
#include "ExtendedInputOutput.h"
using namespace ExtIO;

int AnalogMultiplex::digitalRead(pin_t pin)
{
    setMuxAddress(pinToMuxAddress(pin));
    return ExtIO::digitalRead(analogPin);
}
analog_t AnalogMultiplex::analogRead(pin_t pin)
{
    setMuxAddress(pinToMuxAddress(pin));
    ExtIO::analogRead(analogPin);
    return ExtIO::analogRead(analogPin);
}
void AnalogMultiplex::begin()
{
    ExtIO::pinMode(analogPin, INPUT);
    for (uint8_t i = 0; i < nb_addressPins; i++) {
        Serial.println(addressPins[i]);
        ExtIO::pinMode(addressPins[i], OUTPUT);
    }
}

uint8_t AnalogMultiplex::pinToMuxAddress(pin_t pin)
{
    return pin & (nb_addresses - 1);
}
void AnalogMultiplex::setMuxAddress(uint8_t address)
{
    Serial.print("Address: ");
    Serial.println(address, BIN);
    for (uint8_t i = 0; i < nb_addressPins; i++) {
        ExtIO::digitalWrite(addressPins[i], address & (1 << i));
        Serial.print("\ti = ");
        Serial.println(i);
        Serial.print("\taddress & (1 << i) = ");
        Serial.println(address & (1 << i), BIN);
    }
}
