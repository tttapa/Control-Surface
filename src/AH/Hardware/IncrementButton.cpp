#include "IncrementButton.hpp"

BEGIN_AH_NAMESPACE

IncrementButton::State IncrementButton::updateImplementation() {
    Button::State incrState = button.update();

    if (incrState == Button::Released) {
        // Button released, don't do anything
        // This one is first to minimize overhead
        // because most of the time, the button will
        // be released
        return Nothing;
    } else if (incrState == Button::Rising) {
        auto res = longPressState == LongPress ? ReleasedLong : ReleasedShort;
        longPressState = Initial;
        return res;
    } else if (incrState == Button::Falling) {
        return IncrementShort;
    } else { // if (incrState == Button::Pressed)
        auto now = millis();
        if (longPressState == LongPress) {
            // still long pressed
            if (now - longPressRepeat >= LONG_PRESS_REPEAT_DELAY) {
                longPressRepeat += LONG_PRESS_REPEAT_DELAY;
                return IncrementHold;
            }
        } else if (button.stableTime(now) >= LONG_PRESS_DELAY) {
            // long press starts
            longPressState = LongPress;
            longPressRepeat = now;
            return IncrementLong;
        }
    }
    return Nothing;
}

END_AH_NAMESPACE
