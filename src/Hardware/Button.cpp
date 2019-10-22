#include "Button.hpp"

BEGIN_CS_NAMESPACE

using namespace ExtIO;

Button::Button(pin_t pin) : pin(pin) {}

void Button::begin() { ExtIO::pinMode(pin, INPUT_PULLUP); }

void Button::invert() { invertState = true; }

#ifndef INDIVIDUAL_BUTTON_INVERT
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

const char *Button::getName(Button::State state) {
    switch (state) {
        case Button::Pressed: return "Pressed";
        case Button::Released: return "Released";
        case Button::Falling: return "Falling";
        case Button::Rising: return "Rising";
        default: return ""; // Keeps the compiler happy
    }
}

unsigned long Button::stableTime() { return millis() - prevBounceTime; }

END_CS_NAMESPACE