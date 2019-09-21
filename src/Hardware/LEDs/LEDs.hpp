/* ✔ */

#pragma once

#include <Def/Def.hpp>
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class for collections of LEDs that can display ranges.
 * 
 * @tparam  N
 *          The number of LEDs in the collection.
 */
template <uint8_t N>
class LEDs {
  public:
    /**
     * @brief   Create a LEDs object.
     * 
     * @param   ledPins
     *          An array of pins with the LEDs connected.
     */
    LEDs(const PinList<N> &ledPins) : ledPins(ledPins) {}

    /**
     * @brief   Initialize (set LED pins as outputs).
     */
    void begin() const {
        for (const pin_t &pin : ledPins)
            ExtIO::pinMode(pin, OUTPUT);
    }

    /**
     * @brief   Turn on a range of the LEDs.
     * 
     * @param   startOn
     *          The first LED of the range to turn on (the LEDs before this one
     *          are turned off).
     * @param   startOff
     *          The first LED after the range to turn off.
     */
    void displayRange(uint8_t startOn, uint8_t startOff) const {
        for (uint8_t pin = 0; pin < startOn; pin++)
            ExtIO::digitalWrite(ledPins[pin], LOW);
        for (uint8_t pin = startOn; pin < startOff; pin++)
            ExtIO::digitalWrite(ledPins[pin], HIGH);
        for (uint8_t pin = startOff; pin < N; pin++)
            ExtIO::digitalWrite(ledPins[pin], LOW);
    }

    /**
     * @brief   Turn on a single LED, and turn off all others.
     * 
     * @param   led
     *          The LED to turn on.
     */
    void displayDot(uint8_t led) const { displayRange(led, led + 1); }

    /**
     * @brief   Turn off all LEDs.
     */
    void clear() const {
        for (uint8_t pin = 0; pin < N; pin++)
            ExtIO::digitalWrite(ledPins[pin], LOW);
    }

  private:
    const PinList<N> ledPins;
};

END_CS_NAMESPACE