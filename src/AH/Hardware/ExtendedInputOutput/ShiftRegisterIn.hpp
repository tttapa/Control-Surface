/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include "ExtendedInputOutput.hpp"
#include "StaticSizeExtendedIOElement.hpp"
#include <AH/Containers/Array.hpp>
#include <stdlib.h>

BEGIN_AH_NAMESPACE

/**
 * @brief   A class for reading multiplexed analog inputs.
 *          Supports 74HC4067, 74HC4051, etc.
 * 
 * You can use many multiplexers on the same address lines if each of the 
 * multiplexers has a different enable line.
 * 
 * @tparam  N 
 *          The number of addressable pins.
 * 
 * @ingroup AH_ExtIO
 */
template <uint8_t N>
class ShiftRegisterIn : public StaticSizeExtendedIOElement<N> {
  public:
    /**
     * @brief   Create a new ShiftRegisterIn object on the given pins.
     * 
     * @param   dataPin
     *          The analog input pin connected to the output of the multiplexer.
     * @param   addressPins
     *          An array of the pins connected to the address lines of the
     *          multiplexer. (Labeled S0, S1, S2 ... in the datasheet.)
     * @param   enablePin
     *          The digital output pin connected to the enable pin of the
     *          multiplexer. (Labeled Ē in the datasheet.)  
     *          If you don't need the enable pin, you can use NO_PIN, which is 
     *          the default.
     */
    ShiftRegisterIn(pin_t dataPin, pin_t clockPin, pin_t clockEnablePin, 
                    pin_t loadPin)
        : dataPin(dataPin), clockPin(clockPin), clockEnablePin(clockEnablePin), 
          loadPin(loadPin){

         }

    /**
     * @brief   Set the pin mode of the analog input pin.  
     *          This allows you to enable the internal pull-up resistor, for
     *          use with buttons or open-collector outputs.
     * 
     * @note    This applies to all pins of this multiplexer.  
     *          This affects all pins of the multiplexer, because it has only
     *          a single common pin.  
     * 
     * @param   pin
     *          (Unused)
     * @param   mode
     *          The new mode of the input pin: 
     *          either INPUT or INPUT_PULLUP.
     */
    void pinMode(pin_t pin, PinMode_t mode) override;

    /**
     * @copydoc pinMode
     */
    void pinModeBuffered(pin_t pin, PinMode_t mode) override;

    /**
     * @brief   The digitalWrite function is not implemented because writing an
     *          output to a multiplexer is not useful.
     */
    void digitalWrite(pin_t, PinStatus_t) override // LCOV_EXCL_LINE
        __attribute__((deprecated)) {}             // LCOV_EXCL_LINE

    /**
     * @copydoc digitalWrite
     */
    void digitalWriteBuffered(pin_t, PinStatus_t) override // LCOV_EXCL_LINE
        __attribute__((deprecated)) {}                     // LCOV_EXCL_LINE

    /**
     * @brief   Read the digital state of the given input.
     * 
     * @param   pin
     *          The multiplexer's pin number to read from.
     */
    int digitalRead(pin_t pin) override;

    /**
     * @copydoc digitalRead
     */
    int digitalReadBuffered(pin_t pin) override;

    /**
     * @brief   Read the analog value of the given input.
     * 
     * @param   pin
     *          The multiplexer's pin number to read from.
     */
    analog_t analogRead(pin_t pin) override;

    /**
     * @copydoc analogRead
     */
    analog_t analogReadBuffered(pin_t pin) override;

    /**
     * @brief   The analogWrite function is not implemented because writing an
     *          output to a multiplexer is not useful.
     */
    void analogWrite(pin_t, analog_t) override // LCOV_EXCL_LINE
        __attribute__((deprecated)) {}         // LCOV_EXCL_LINE

    /**
     * @copydoc analogWrite
     */
    void analogWriteBuffered(pin_t, analog_t) override // LCOV_EXCL_LINE
        __attribute__((deprecated)) {}                 // LCOV_EXCL_LINE

    /**
     * @brief   Initialize the multiplexer: set the pin mode of the address pins
     *          and the enable pin to output mode.
     */
    void begin() override;

    void read();

    /**
     * @brief   No periodic updating of the state is necessary, all actions are 
     *          carried out when the user calls analogRead or digitalRead.
     */
    void updateBufferedOutputs() override {} // LCOV_EXCL_LINE

    void updateBufferedInputs() override;

  private:
    const pin_t dataPin;
    const pin_t clockPin;
    const pin_t clockEnablePin;
    const pin_t loadPin; 

    uint8_t buffer;

    /**
     * @brief   Enable the shift register.
     */
    void prepareReading();

    /**
     * @brief   Disable the shift register.
     */
    void afterReading();
};

/**
 * @brief   An alias for ShiftRegisterIn<3> to use with CD74HC165 shift 
 *          registers.
 * 
 * @ingroup AH_ExtIO
 */
using CD74HC165 = ShiftRegisterIn<8>;

// -------------------------------------------------------------------------- //

template <uint8_t N>
void ShiftRegisterIn<N>::pinMode(pin_t, PinMode_t mode) {
    ExtIO::pinMode(dataPin, mode);
}

template <uint8_t N>
void ShiftRegisterIn<N>::pinModeBuffered(pin_t, PinMode_t mode) {
    ShiftRegisterIn<N>::pinMode(dataPin, mode);
}

template <uint8_t N>
int ShiftRegisterIn<N>::digitalRead(pin_t pin) {
    return bitRead(buffer, pin);
}

template <uint8_t N>
int ShiftRegisterIn<N>::digitalReadBuffered(pin_t pin) {
    return bitRead(buffer, pin);
}

template <uint8_t N>
analog_t ShiftRegisterIn<N>::analogRead(pin_t pin) {
    return pin;
}

template <uint8_t N>
analog_t ShiftRegisterIn<N>::analogReadBuffered(pin_t pin) {
    return ShiftRegisterIn<N>::analogRead(pin);
}

template <uint8_t N>
void ShiftRegisterIn<N>::begin() {
    ExtIO::pinMode(clockPin, OUTPUT);
    ExtIO::pinMode(clockEnablePin, OUTPUT);
    ExtIO::pinMode(loadPin, OUTPUT);

    ExtIO::digitalWrite(clockPin, LOW);
    ExtIO::digitalWrite(loadPin, HIGH);
    ExtIO::digitalWrite(clockEnablePin, HIGH);
}

template <uint8_t N>
void ShiftRegisterIn<N>::updateBufferedInputs() {
    prepareReading();
    
    buffer = shiftIn(dataPin, clockPin, LSBFIRST);

    afterReading();
}

template <uint8_t N>
void ShiftRegisterIn<N>::prepareReading() {
    ExtIO::digitalWrite(loadPin, LOW);
    ExtIO::digitalWrite(loadPin, HIGH);
    ExtIO::digitalWrite(clockEnablePin, LOW);
}

template <uint8_t N>
void ShiftRegisterIn<N>::afterReading() {
    ExtIO::digitalWrite(clockEnablePin, HIGH);
}

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
