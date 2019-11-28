#include "Button.hpp"

AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

BEGIN_AH_NAMESPACE

using namespace ExtIO;

Button::Button(pin_t pin) : pin(pin) {}

void Button::begin() { ExtIO::pinMode(pin, INPUT_PULLUP); }

void Button::invert() { invertState = true; }

#ifndef AH_INDIVIDUAL_BUTTON_INVERT
bool Button::invertState = false;
#endif

Button::State Button::update() {
    // read the button state and invert it if "invertState" is true
    bool input = ExtIO::digitalRead(pin) ^ invertState;
    bool prevState = debouncedState & 0b01;
    unsigned long now = millis();
    if (now - prevBounceTime > debounceTime) { // wait for state to stabilize
        debouncedState = static_cast<State>((prevState << 1) | input);
    } else {
        debouncedState = static_cast<State>((prevState << 1) | prevState);
    }
    if (input != prevInput) { // Button is pressed, released or bounces
        prevBounceTime = now;
        prevInput = input;
    }
    return debouncedState;
}

Button::State Button::getState() const { return debouncedState; }

const __FlashStringHelper *Button::getName(Button::State state) {
    switch (state) {
        case Button::Pressed: return F("Pressed");
        case Button::Released: return F("Released");
        case Button::Falling: return F("Falling");
        case Button::Rising: return F("Rising");
        default: return F("<invalid>"); // Keeps the compiler happy
    }
}

unsigned long Button::previousBounceTime() const { return prevBounceTime; }

unsigned long Button::stableTime(unsigned long now) const {
    return now - previousBounceTime();
}

unsigned long Button::stableTime() const { return stableTime(millis()); }

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()