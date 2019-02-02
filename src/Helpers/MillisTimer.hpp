#pragma once

#include <Arduino.h>

using timefunction = unsigned long (*)();

template <timefunction time = micros>
class Timer {
  public:
    Timer(unsigned long interval) : interval(interval) {}
    void begin() { previous = time(); }
    explicit operator bool() {
        if (time() - previous >= interval) {
            previous += interval;
            return true;
        }
        return false;
    }

  private:
    const unsigned long interval;
    unsigned long previous = 0;
};