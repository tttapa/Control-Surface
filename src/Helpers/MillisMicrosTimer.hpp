#pragma once

#include <Arduino.h>

/// A function type that returns a time value.
using timefunction = unsigned long (*)();

/// @addtogroup    Utilities
/// @{

/**
 * @brief   A class for easily managing timed events. A wrapper for "Blink 
 *          Without Delay".
 * 
 * @tparam time
 *         The time function to use.
 */
template <timefunction time = micros>
class Timer {
  public:
    /**
     * @brief   Constructor.
     * @param   interval
     *          The interval between two events.
     */
    Timer(unsigned long interval) : interval(interval) {
#ifdef ARDUINO
        begin();
#endif
    }
    /// Initialize the timer.
    void begin() { previous = time() - interval; }
    /// Update the timer and return true if the event should fire.
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

/// @}