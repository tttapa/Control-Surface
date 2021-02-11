/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/Hardware/LEDs/LEDs.hpp>

BEGIN_AH_NAMESPACE

/**
 * @brief   An enumeration type to set an LED display to either bar or dot mode.
 */
enum class DotBarMode : bool {
    Bar = false, ///< Turn on a range of LEDs up to the active LED.
    Dot = true,  ///< Turn on only the active LED
};

/**
 * @brief   A class for LED bars.
 * 
 * @tparam  N 
 *          The number of LEDs in the bar.
 * 
 * @ingroup AH_HardwareUtils
 */
template <uint16_t N>
class DotBarDisplayLEDs : public LEDs<N> {
  public:
    /// Constructor from list of pins.
    DotBarDisplayLEDs(const PinList<N> &ledPins) : LEDs<N>{ledPins} {}

    /** 
     * @brief   Display the given number of LEDs on the LED bar.
     * 
     * @param   value
     *          The number of the LED to activate.
     */
    void display(uint16_t value) const {
        if (value == 0)
            this->clear();
        else if (mode == DotBarMode::Bar)
            this->displayRange(0, value);
        else
            this->displayDot(value - 1);
    }

    /**
     * @brief   Display the given fraction of the LED bar.
     * 
     * @param   value 
     *          The fraction of the LED bar to display.
     */
    void display(float value) const { display(uint16_t(value * (N + 1))); }

    /// Get the dot/bar mode.
    DotBarMode getMode() const { return mode; }

    /**
     * @brief   Set the mode to either dot or bar mode.
     * 
     * @param   mode 
     *          The mode.
     */
    void setMode(DotBarMode mode) { this->mode = mode; }

    /// Set the mode to dot mode.
    void dotMode() { setMode(DotBarMode::Dot); }

    /// Set the mode to bar mode.
    void barMode() { setMode(DotBarMode::Bar); }

    /// Toggle the dot/bar mode.
    void toggleMode() { getMode() == DotBarMode::Bar ? dotMode() : barMode(); }

  private:
    DotBarMode mode = DotBarMode::Bar;
};

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
