#pragma once

#include "../Hardware/ExtendedInputOutput/ExtendedInputOutput.h"
#include "../Settings/SettingsWrapper.h"

class Button {
  public:
    /**
     * @brief Construct a new Button object.
     *
     * @param pin The digital pin to read from. The internal pull-up resistor
     * will be enabled.
     */
    Button(pin_t pin); // Constructor

    /**
     * @brief Invert the state of all buttons (button pressed is HIGH instead of
     * LOW).
     *
     * @note This impacts **all** Button objects.
     */
    static void invert();

    enum State {
        Pressed = 0b00,
        Released = 0b11,
        Falling = 0b10,
        Rising = 0b01
    };
    /**
     * @brief Get the state of the button.
     *
     * <pre>
  Debounce time: ├────┤

  Raw input:
    HIGH  ──────┐      ┌──────┐ ┌─┐      ┌─┐ ┌──────┐ ┌────────
    LOW         └──────┘      └─┘ └──────┘ └─┘      └─┘
                ├────┤        ├─┼─┼────┤ ├─┼─┼────┤ ├─┼────┤

  Debounced output:
    HIGH  ──────┐      ┌──────┐          ┌──────────┐      ┌───
    LOW         └──────┘      └──────────┘          └──────┘
  </pre>
     * <pre>
    HIGH  ────────────────┐                  ┌─────────────────
    LOW                   └──────────────────┘
            RELEASED   FALLING   PRESSED   RISING
  </pre>
     *
     * @return State
     *         The state of the button, either Button::PRESSED,
  Button::RELEASED, Button::FALLING or Button::RISING.
     *         The button is debounced, the debounce time can be set in
  ../Settings/Settings.h
     */
    State getState(); // Check if the button state changed

  private:
    const pin_t pin;

    bool prevState = HIGH;
    bool debouncedState = HIGH;
    unsigned long prevBounceTime = 0;

    static bool invertState;

    constexpr static unsigned long debounceTime =
        BUTTON_DEBOUNCE_TIME; // Edit this in ../Settings/Settings.h
};