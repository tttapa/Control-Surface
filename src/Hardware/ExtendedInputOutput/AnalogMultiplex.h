#pragma once

#include "../../Helpers/Copy.hpp"
#include "ExtendedIOElement.h"
#include <stdlib.h>

/**
 * @brief   A class for reading multiplexed analog inputs.
 * 
 *          Supports 74HC4067, 74HC4051, etc.
 */
class AnalogMultiplex : public ExtendedIOElement {
  public:
    /**
     * @brief   Create a new AnalogMultiplex object on the given pins.
     * 
     * @param   analogPin
     *          The analog input pin connected to the output of the multiplexer.
     * @param   addressPins
     *          A list of the pins connected to the address lines of the
     *          multiplexer.
     * @note    The list of address pins is copied. This means that it is safe
     *          to initialize it using a brace-enclosed initializer list.
     */
    template <class P, size_t N>
    AnalogMultiplex(pin_t analogPin, const P (&addressPins)[N])
        : ExtendedIOElement(1 << N), analogPin(analogPin), nb_addressPins(N) {
        this->addressPins = new pin_t[N];
        copy(this->addressPins, addressPins);
        begin();
    }
    ~AnalogMultiplex() { delete[] addressPins; }

    /**
     * @brief   Set the pin mode of the analog input pin.  
     *          This allows you to enable the internal pull-up resistor, for
     *          use with buttons or open-collector outputs.
     * 
     * @note    This applies to all pins of this multiplexer.  
     *          It is not possible to enable the pull-up resistor on some of
     *          the inputs and not on others.  
     * 
     * @param   (unused)
     * @param   mode
     *          The new mode of the analog input pin: 
     *          either INPUT or INPUT_PULLUP.
     */
    void pinMode(pin_t, uint8_t mode) override;

    /**
     * @brief   The digitalWrite function is not implemented because writing an
     *          output to a multiplexer is not useful.
     */
    void digitalWrite(pin_t pin, uint8_t val) override __attribute__((deprecated)) {}

    /**
     * @brief   Read the digital state of the given input.
     * 
     * @param   pin
     *          The multiplexer's pin number to read from.
     */
    int digitalRead(pin_t pin) override;

    /**
     * @brief   Read the analog value of the given input.
     * 
     * @param   pin
     *          The multiplexer's pin number to read from.
     */
    analog_t analogRead(pin_t pin) override;

    /**
     * @brief   The analogWrite function is not implemented because writing an
     *          output to a multiplexer is not useful.
     */
    void analogWrite(pin_t pin, analog_t val) override __attribute__((deprecated)) {}

    /**
     * @brief   Initialize the multiplexer.
     */
    void begin() override;

    void update() override;

  private:
    const pin_t analogPin;
    pin_t *addressPins = nullptr;
    const size_t nb_addressPins;
    /**
     * @brief   Write the pin number/address to the address pins of the 
     *          multiplexer. 
     */
    void setMuxAddress(uint8_t address);
};