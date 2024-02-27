/* ✔ */

#pragma once

#include "Button.hpp"

BEGIN_AH_NAMESPACE

/**
 * @brief   A class for buttons that increment some counter or setting.
 * 
 * It behaves the same way as a computer keyboard: when you press the button, 
 * it increments the counter once. If you keep on pressing it for longer than
 * a certain threshold, it keeps on incrementing at a faster rate, until you
 * release it.
 * 
 * @ingroup AH_HardwareUtils
 */
class IncrementButton {
  public:
    /** 
     * @brief   Create a IncrementButton.
     * 
     * @param   button
     *          The button to read from.  
     *          The button is copied.
     */
    IncrementButton(const Button &button) : button(button) {}

    /// @see     Button::begin
    void begin() { button.begin(); }

    /**
     * @brief   An enumeration of the different actions to be performed by the
     *          counter.
     * @todo    Add states for initial press.
     */
    enum State {
        Nothing = 0,    ///< The counter must not be incremented.
        IncrementShort, ///< The counter must be incremented (after short press).
        IncrementLong,  ///< The counter must be incremented (after long press).
        IncrementHold,  ///< The counter must be incremented (still pressed).
        ReleasedShort,  ///< The button was released after a short press.
        ReleasedLong,   ///< The button was released after a long press.
    };

    /**
     * @brief   Update and return the state of the increment button.
     */
    State update() { return state = updateImplementation(); }

    /**
     * @brief   Return the state of the increment button without updating it.
     * 
     * Returns the same value as the last @ref update call.
     */
    State getState() const { return state; }

    /// @see    Button::invert
    void invert() { button.invert(); }

  protected:
    State updateImplementation();

  private:
    Button button;

    enum {
        Initial,
        LongPress,
    } longPressState = Initial;
    unsigned long longPressRepeat;

    State state = Nothing;
};

END_AH_NAMESPACE
