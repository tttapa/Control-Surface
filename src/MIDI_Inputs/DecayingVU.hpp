#pragma once

#include <Arduino.h> // millis

class DecayingVU {
  protected:
    DecayingVU(unsigned long decayTime) : decayTime(decayTime) {}

  private:
    const unsigned long decayTime;
    unsigned long prevDecayTime = 0;

    /// Automatically decay the VU meter value by one step (if it is greater
    /// than zero).
    virtual void decayAll() = 0;

    /// Display the VU meter to the output.
    virtual void display() const = 0;

  protected:
    void resetDecayTimer() { prevDecayTime = millis(); }

  public:
    void update() {
        if (decayTime && ((millis() - prevDecayTime) >= decayTime)) {
            prevDecayTime += decayTime;
            decayAll();
            display();
        }
    }

    constexpr static unsigned int NO_DECAY = 0;
};
