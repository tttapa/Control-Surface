#include "IncrementDecrementButtons.hpp"

AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

BEGIN_AH_NAMESPACE

IncrementDecrementButtons::State
IncrementDecrementButtons::updateImplementation() {
    Button::State incrState = incrementButton.update();
    Button::State decrState = decrementButton.update();

    if (decrState == Button::Released && incrState == Button::Released) {
        // Both released
    } else if ((decrState == Button::Rising && incrState == Button::Released) ||
               (incrState == Button::Rising && decrState == Button::Released) ||
               (incrState == Button::Rising && decrState == Button::Rising)) {
        // One released, the other rising → nothing
        // now both released, so go to initial state
        longPressState = Initial;
    } else if (incrState == Button::Falling && decrState == Button::Falling) {
        // Both falling → reset
        // (rather unlikely, but just in case)
        longPressState = AfterReset;
        return Reset;
    } else if (incrState == Button::Falling) {
        if (decrState == Button::Pressed) {
            // One pressed, the other falling → reset
            longPressState = AfterReset;
            return Reset;
        } else {
            // Increment falling, the other released → increment
            return IncrementShort;
        }
    } else if (decrState == Button::Falling) {
        if (incrState == Button::Pressed) {
            // One pressed, the other falling → reset
            longPressState = AfterReset;
            return Reset;
        } else {
            // Decrement falling, the other released → decrement
            return DecrementShort;
        }
    } else if (incrState == Button::Pressed && decrState == Button::Pressed) {
        // Both pressed → nothing
    } else if (longPressState != AfterReset && incrState == Button::Pressed) {
        // Not reset and increment pressed → long press?
        auto now = millis();
        if (longPressState == LongPress) {
            if (now - longPressRepeat >= LONG_PRESS_REPEAT_DELAY) {
                longPressRepeat += LONG_PRESS_REPEAT_DELAY;
                return IncrementHold;
            }
        } else if (incrementButton.stableTime() >= LONG_PRESS_DELAY) {
            longPressState = LongPress;
            longPressRepeat = now;
            return IncrementLong;
        }
    } else if (longPressState != AfterReset && decrState == Button::Pressed) {
        // Not reset and decrement pressed → long press?
        auto now = millis();
        if (longPressState == LongPress) {
            if (now - longPressRepeat >= LONG_PRESS_REPEAT_DELAY) {
                longPressRepeat += LONG_PRESS_REPEAT_DELAY;
                return DecrementHold;
            }
        } else if (decrementButton.stableTime() >= LONG_PRESS_DELAY) {
            longPressState = LongPress;
            longPressRepeat = now;
            return DecrementLong;
        }
    }
    return Nothing;
}

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
