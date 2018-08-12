#pragma once

#include <Hardware/LEDs/LEDs.hpp>

template <uint8_t N>
class VULEDs : public LEDs<N> {
  public:
    VULEDs(const PinList<N> &ledPins) : LEDs<N>(ledPins) {}
    void display(uint8_t value) const {
        if (value == 0)
            this->clear();
        else if (bar)
            this->displayRange(0, value);
        else
            this->displayDot(value - 1);
    }
    void display(float value) const { display(uint8_t(value * (N + 1))); }

    void dotMode() { bar = false; }
    void barMode() { bar = true; }

  private:
    bool bar = true;
};