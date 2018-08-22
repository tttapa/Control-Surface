#pragma once

#include <Hardware/LEDs/LEDs.hpp>

template <uint8_t N>
class DotBarDisplayLEDs : public LEDs<N> {
  public:
    DotBarDisplayLEDs(const PinList<N> &ledPins) : LEDs<N>(ledPins) {}

    enum VUMode : bool {
        Bar = false,
        Dot = true,
    };

    void display(uint8_t value) const {
        if (value == 0)
            this->clear();
        else if (mode == Bar)
            this->displayRange(0, value);
        else
            this->displayDot(value - 1);
    }
    void display(float value) const { display(uint8_t(value * (N + 1))); }

    void setMode(VUMode mode) { this->mode = mode; }
    void dotMode() { setMode(Dot); }
    void barMode() { setMode(Bar); }

  private:
    VUMode mode = Bar;
};