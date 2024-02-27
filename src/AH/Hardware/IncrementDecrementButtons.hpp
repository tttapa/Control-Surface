/* ✔ */

#pragma once

#include "Button.hpp"

BEGIN_AH_NAMESPACE

/**
 * @brief   A class for buttons that increment and decrement some counter or 
 *          setting.
 * 
 * It behaves the same way as a computer keyboard: when you press the increment
 * (decrement) button, it increments (decrements) the counter once. 
 * If you keep on pressing it for longer than a certain threshold, it keeps on 
 * incrementing (decrementing) at a faster rate, until you release it.  
 * If both the increment and the decrement button are pressed at once, it resets
 * the counter.
 * 
 * @ingroup AH_HardwareUtils
 */
class IncrementDecrementButtons {
  public:
    /** 
     * @brief   Create a IncrementDecrementButtons object.
     * 
     * @param   incrementButton
     *          The button to increment the counter.  
     *          The button is copied.
     * @param   decrementButton
     *          The button to decrement the counter.  
     *          The button is copied.
     */
    IncrementDecrementButtons(const Button &incrementButton,
                              const Button &decrementButton)
        : incrementButton(incrementButton), decrementButton(decrementButton) {}

    /// @see     Button::begin
    void begin() {
        incrementButton.begin();
        decrementButton.begin();
    }

    /**
     * @brief   An enumeration of the different actions to be performed by the
     *          counter.
     * @todo    Add states for initial press.
     */
    enum State {
        Nothing = 0,    ///< The counter should not be incremented.
        IncrementShort, ///< The counter must be incremented (after short press).
        IncrementLong,  ///< The counter must be incremented (after long press).
        IncrementHold,  ///< The counter must be incremented (still pressed).
        DecrementShort, ///< The counter must be decremented (after short press).
        DecrementLong,  ///< The counter must be decremented (after long press).
        DecrementHold,  ///< The counter must be decremented (still pressed).
        Reset,          ///< The counter should be reset to the initial value.
    };

    /**
     * @brief   Update and return the state of the increment/decrement button.
     */
    State update() { return state = updateImplementation(); }

    /**
     * @brief   Return the state of the increment/decrement button without 
     *          updating it.
     * 
     * Returns the same value as the last @ref update call.
     */
    State getState() const { return state; }

    /// @see    Button::invert
    void invert() {
        incrementButton.invert();
        decrementButton.invert();
    }

  protected:
    State updateImplementation();

  private:
    Button incrementButton;
    Button decrementButton;

    enum {
        Initial,
        LongPress,
        AfterReset,
    } longPressState = Initial;
    unsigned long longPressRepeat;
    State state = Nothing;
};

END_AH_NAMESPACE
