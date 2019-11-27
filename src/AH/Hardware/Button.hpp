/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <AH/Settings/SettingsWrapper.hpp>

BEGIN_AH_NAMESPACE

/**
 * @brief   A class for reading and debouncing buttons and switches.
 * 
 * @ingroup AH_HardwareUtils
 */
class Button {
  public:
    /**
     * @brief   Construct a new Button object.
     * 
     * **This constructor should not be used.**  
     * It is just a way to easily create arrays of buttons, and initializing
     * them later.
     */
    Button() : pin(NO_PIN) {}

    /**
     * @brief   Construct a new Button object.
     *
     * @param   pin
     *          The digital pin to read from. The internal pull-up resistor
     *          will be enabled when `begin` is called.
     */
    Button(pin_t pin);

    /// @brief   Initialize (enable the internal pull-up resistor).
    void begin();

    /**
     * @brief   Invert the state of all buttons, or of this specific button 
     *          (button pressed is `HIGH` instead of `LOW`).
     *
     * @note    This affects **all** Button objects if 
     *          `AH_INDIVIDUAL_BUTTON_INVERT` is not defined.
     * 
     * @see     AH_INDIVIDUAL_BUTTON_INVERT
     */
    AH_INDIVIDUAL_BUTTON_INVERT_STATIC void invert();

    /// @brief   An enumeration of the different states a button can be in.
    enum State {
        Pressed = 0b00,  /// < Input went from low to low   (0,0)
        Released = 0b11, /// < Input went from high to high (1,1)
        Falling = 0b10,  /// < Input went from high to low  (1,0)
        Rising = 0b01    /// < Input went from low to high  (0,1)
    };

    /**
     * @brief   Read the button and return its new state.
     *
     * The button is debounced, the debounce time can be set in
     * Settings.hpp: #BUTTON_DEBOUNCE_TIME.
     *
     * ```
     * Debounce time: ├────┤
     *
     * Raw input:
     *    HIGH  ──────┐      ┌──────┐ ┌─┐      ┌─┐ ┌──────┐ ┌────────
     *    LOW         └──────┘      └─┘ └──────┘ └─┘      └─┘
     *                ├────┤ ├────┤ ├─┼─┼────┤ ├─┼─┼────┤ ├─┼────┤
     *
     * Debounced output:
     *    HIGH  ──────┐      ┌──────┐          ┌──────────┐      ┌───
     *    LOW         └──────┘      └──────────┘          └──────┘
     *
     * States:
     *    HIGH  ────────────────┐                  ┌─────────────────
     *    LOW                   └──────────────────┘
     *            RELEASED   FALLING   PRESSED   RISING
     * ```
     *
     * @return  The state of the button, either Button::PRESSED,
     *          Button::RELEASED, Button::FALLING or Button::RISING.
     */
    State update();

    /**
     * @brief   Get the state of the button, without updating it.
     *          Returns the same value as the last call to @ref update.
     *
     * @return  The state of the button, either Button::PRESSED,
     *          Button::RELEASED, Button::FALLING or Button::RISING.
     */
    State getState() const;

    /// @brief   Return the name of the state as a string.
    static const __FlashStringHelper *getName(State state);

    /// Return the time point (in milliseconds) when the button last bounced.
    unsigned long previousBounceTime() const;

    /// Return the time (in milliseconds) that the button has been stable for,
    /// compared to the given time point.
    unsigned long stableTime(unsigned long now) const;
    
    /// Return the time (in milliseconds) that the button has been stable for.
    unsigned long stableTime() const;

  private:
    pin_t pin;

    bool prevInput = HIGH;
    State debouncedState = Released;
    unsigned long prevBounceTime = 0;

#ifdef AH_INDIVIDUAL_BUTTON_INVERT // Edit this in Settings/Settings.hpp
    bool invertState = false;
#else
    static bool invertState;
#endif

    /// Edit this in Settings.hpp
    /// @see    BUTTON_DEBOUNCE_TIME
    constexpr static unsigned long debounceTime = BUTTON_DEBOUNCE_TIME;
};

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
