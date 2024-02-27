/* ✔ */

#pragma once

#include "Button.hpp"

BEGIN_AH_NAMESPACE

/**
 * @brief   Class for detecting short/long button presses and double clicks.
 */
class MultiPurposeButton {
  public:
    MultiPurposeButton(pin_t pin) : button(pin) {}

    enum Event {
        None,              ///< Nothing changed
        PressStart,        ///< The button was just pressed
        ShortPressRelease, ///< The button was released after a short press
        LongPress,         ///< The button has been pressed for some time
        LongPressRelease,  ///< The button was released after a long press
        MultiPress,        ///< The button was pressed in quick succession of
                           ///< the previous release.
        MultiPressDone,    ///< The button has been released for long enough to
                           ///< rule out another MultiPress.
    };

    /// @see @ref Button::begin()
    void begin() { button.begin(); }

    /// Read the button state and return the @ref Event (if any).
    Event update() {
        Event event = None;
        auto now = millis();
        auto stableTime = button.stableTime(now);
        switch (button.update()) {
            case Button::Released: {
                if (multiPressCountNew > 0 && stableTime > multiPressDelay) {
                    multiPressCount = multiPressCountNew;
                    multiPressCountNew = 0;
                    event = MultiPressDone;
                }
            } break;
            case Button::Falling: {
                event = (stableTime <= multiPressDelay) ? MultiPress //
                                                        : PressStart;
                multiPressCountNew += event == MultiPress;
            } break;
            case Button::Pressed: {
                if (not longPress && stableTime >= longPressDelay) {
                    event = LongPress;
                    longPress = true;
                }
            } break;
            case Button::Rising: {
                event = longPress ? LongPressRelease : ShortPressRelease;
                longPress = false;
            } break;
        }
        return event;
    }

    /// Get the number of times the button was pressed in quick succession
    /// (after MultiPressDone), this is the final count.
    uint8_t getMultiPressCount() const { return multiPressCount; }
    /// Get the number of times the button was pressed in quick succession,
    /// while the button is being pressed (before MultiPressDone).
    /// The count could still increase if the user keeps on pressing the button.
    uint8_t getCurrentMultiPressCount() const { return multiPressCountNew; }

    /// Get the number of milliseconds after which a press is considered a long
    /// press.
    unsigned long getLongPressDelay() const { return longPressDelay; }
    /// Set the number of milliseconds after which a press is considered a long
    /// press.
    void setLongPressDelay(unsigned long longPressDelay) {
        this->longPressDelay = longPressDelay;
    }

    /// Get the number of milliseconds between multipresses.
    unsigned long getMultiPressDelay() const { return multiPressDelay; }
    /// Set the number of milliseconds between multipresses.
    void setMultiPressDelay(unsigned long multiPressDelay) {
        this->multiPressDelay = multiPressDelay;
    }

    /// @see @ref Button::previousBounceTime() const
    unsigned long previousBounceTime() const {
        return button.previousBounceTime();
    }
    /// @see @ref Button::stableTime() const
    unsigned long stableTime() const { return button.stableTime(); }
    /// @see @ref Button::stableTime(unsigned long) const
    unsigned long stableTime(unsigned long now) const {
        return button.stableTime(now);
    }

    /// Get the number of milliseconds the button has been pressed for. Returns
    /// 0 if the button is not currently pressed.
    unsigned long getPressedTime() const {
        return getButtonState() == Button::Pressed ? stableTime() : 0;
    }
    /// Get the number of milliseconds the button has been pressed for. Returns
    /// 0 if the button is not currently pressed or if the current press is not
    /// a long press (yet).
    unsigned long getLongPressedTime() const {
        return longPress ? stableTime() : 0;
    }

    /// Return the name of the event  as a string.
    static FlashString_t getName(Event ev) { return to_string(ev); }

    /// @see @ref Button::getState() const
    Button::State getButtonState() const { return button.getState(); }
    /// @see @ref Button::invert()
    void invert() { button.invert(); }

  protected:
    Button button;
    unsigned long longPressDelay = 1000;
    unsigned long multiPressDelay = 400;
    bool longPress = false;
    uint8_t multiPressCountNew = 0;
    uint8_t multiPressCount = 0;

  public:
    friend FlashString_t to_string(Event ev) {
        switch (ev) {
            case None: return F("None");
            case PressStart: return F("PressStart");
            case ShortPressRelease: return F("ShortPressRelease");
            case LongPress: return F("LongPress");
            case LongPressRelease: return F("LongPressRelease");
            case MultiPress: return F("MultiPress");
            case MultiPressDone: return F("MultiPressDone");
        }
        return F("<invalid>");
    }
};

END_AH_NAMESPACE
