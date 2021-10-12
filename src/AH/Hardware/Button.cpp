#include "Button.hpp"

AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

BEGIN_AH_NAMESPACE

Button::Button(pin_t pin) : pin(pin) {}

void Button::begin() { ExtIO::pinMode(pin, INPUT_PULLUP); }

void Button::invert() { state.invert = true; }

Button::State Button::update() {
    // Read pin state and current time
    bool input = ExtIO::digitalRead(pin) ^ state.invert;
    unsigned long now = millis();
    // Check if enough time has elapsed after last bounce
    if (state.bouncing)
        state.bouncing = now - state.prevBounceTime <= debounceTime;
    // Shift the debounced state one bit to the left, either appending the
    // new input state if not bouncing, or repeat the old state if bouncing
    bool prevState = state.debounced & 0b01;
    bool newState = state.bouncing ? prevState : input;
    state.debounced = (prevState << 1) | newState;
    // Check if the input changed state (button pressed, released or bouncing)
    if (input != state.prevInput) {
        state.bouncing = true;
        state.prevInput = input;
        state.prevBounceTime = now;
    }
    return getState();
}

Button::State Button::getState() const {
    return static_cast<State>(state.debounced);
}

FlashString_t Button::getName(Button::State state) {
    switch (state) {
        case Button::Pressed: return F("Pressed");
        case Button::Released: return F("Released");
        case Button::Falling: return F("Falling");
        case Button::Rising: return F("Rising");
        default: return F("<invalid>"); // Keeps the compiler happy
    }
}

unsigned long Button::previousBounceTime() const {
    return state.prevBounceTime;
}

unsigned long Button::stableTime(unsigned long now) const {
    return now - previousBounceTime();
}

unsigned long Button::stableTime() const { return stableTime(millis()); }

void Button::setDebounceTime(unsigned long debounceTime) {
    Button::debounceTime = debounceTime;
}

unsigned long Button::getDebounceTime() { return Button::debounceTime; }

unsigned long Button::debounceTime = BUTTON_DEBOUNCE_TIME;

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()