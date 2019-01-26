#pragma once

#include <Hardware/LEDs/LEDs.hpp>

enum class  DotBarMode : bool {
    Bar = false,
    Dot = true,
};

template <uint8_t N>
class DotBarDisplayLEDs : public LEDs<N> {
  public:
    DotBarDisplayLEDs(const PinList<N> &ledPins) : LEDs<N>(ledPins) {}

    void display(uint8_t value) const {
        if (value == 0)
            this->clear();
        else if (mode == DotBarMode::Bar)
            this->displayRange(0, value);
        else
            this->displayDot(value - 1);
    }
    void display(float value) const { display(uint8_t(value * (N + 1))); }

    void setMode(DotBarMode mode) { this->mode = mode; }
    void dotMode() { setMode(DotBarMode::Dot); }
    void barMode() { setMode(DotBarMode::Bar); }

  private:
    DotBarMode mode = DotBarMode::Bar;
};