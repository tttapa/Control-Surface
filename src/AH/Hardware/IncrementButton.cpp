#include "IncrementButton.hpp"

AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

BEGIN_AH_NAMESPACE

IncrementButton::State IncrementButton::updateImplementation() {
    Button::State incrState = button.update();

    if (incrState == Button::Released) {
        // Button released, don't do anything
        // This one is first to minimize overhead
        // because most of the time, the button will
        // be released
    } else if (incrState == Button::Rising) {
        longPressState = Initial;
    } else if (incrState == Button::Falling) {
        return Increment;
    } else { // if (incrState == Button::Pressed)
        if (longPressState == LongPress) {
            // still long pressed
            if (millis() - longPressRepeat >= LONG_PRESS_REPEAT_DELAY) {
                longPressRepeat += LONG_PRESS_REPEAT_DELAY;
                return Increment;
            }
        } else if (button.stableTime() >= LONG_PRESS_DELAY) {
            // long press starts
            longPressState = LongPress;
            longPressRepeat = millis();
            return Increment;
        }
    }
    return Nothing;
}

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
