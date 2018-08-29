#pragma once

#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.h>
#include <Settings/SettingsWrapper.h>

/**
 * @brief   A class for reading and debouncing buttons and switches.
 */
class Button {
  public:
    /**
     * @brief   Construct a new Button object.
     * 
     * This constructor sets the pin to zero, so it is not recommended.
     * It is just a way to easily create arrays of buttons, and initializing
     * them later.
     */
    Button() : pin(0) {}
    /**
     * @brief   Construct a new Button object.
     *
     * @param   pin
     *          The digital pin to read from. The internal pull-up resistor
     *          will be enabled when `begin` is called.
     */
    Button(pin_t pin);

    void begin();

    /**
     * @brief Invert the state of all buttons (button pressed is HIGH instead of
     * LOW).
     *
     * @note This affects **all** Button objects.
     */
    static void invert();

    /**
     * @brief   An enumeration of the different states a button can be in.
     */
    enum State {
        Pressed = 0b00,  /** Input went from low to low   (0,0) */
        Released = 0b11, /** Input went from high to high (1,1) */
        Falling = 0b10,  /** Input went from high to low  (1,0) */
        Rising = 0b01    /** Input went from low to high  (0,1) */
    };

    /**
     * @brief Get the state of the button.
     *
     * The button is debounced, the debounce time can be set in
     * Settings.h: #BUTTON_DEBOUNCE_TIME.
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
    State getState();

    /**
     * @brief   Return the name of the state as a string.
     */
    static const char *getName(State state);

    /** 
     * @brief   Return the time in milliseconds that the button has been stable 
     *          for.
     */
    unsigned long stableTime();

  private:
    pin_t pin;

    bool prevState = HIGH;
    bool debouncedState = HIGH;
    unsigned long prevBounceTime = 0;

    static bool invertState;

    // Edit this in Settings/Settings.h
    constexpr static unsigned long debounceTime = BUTTON_DEBOUNCE_TIME;
};