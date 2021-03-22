/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <AH/Settings/SettingsWrapper.hpp>

BEGIN_AH_NAMESPACE
/**
 * @file Piezo.hpp
 * @brief  Variables used in Piezo velocity sensing
 * 
 * @param maxInputInitial
 *          The max expected input. True max from
 *          most analog input is 1023, but that is rarely reached.   
**/

// Tune these to make it work. If 
#define playDuration 3000 // milliseconds. How long a note should play. 
#define strikeDuration 10 // milliseconds. How long the the arduino should sample for the max velocity.
#define DebounceTime 100 // milliseconds. Debouncing time. Only used in Simple Debouncing

// If this is set low, all hits will register as high velocity. If it is high, all hits will register low velocity.
// Balance it according to your needs. If Auto Adjust is uncommented, set this low.
# define MaxInputInitial 200

// Tune these to avoid double hits. Only used in Fancy Debouncing.
#define secondHitThreshold 1.9 // How much higher the second hit's threshold should be.
#define thresholdDecaySpeed 14 // How fast the hit should return to normal.

/**
 * @brief   A class for reading and debouncing buttons and switches.
 * 
 * @ingroup AH_HardwareUtils
 */
class Piezo {
  public:
    /**
     * @brief   Construct a new Piezo object.
     * 
     * **This constructor should not be used.**  
     * It is just a way to easily create arrays of buttons, and initializing
     * them later.
     */
    Piezo() : pin(NO_PIN) {}

    /**
     * @brief   Construct a new Piezo object.
     *
     * @param   pin
     *          The digital pin to read from. The internal pull-up resistor
     *          will be enabled when `begin` is called.
     */
    Piezo(pin_t pin);

    /// @brief   Initialize (enable the internal pull-up resistor).
    void begin();

    /// @brief   An enumeration of the different states a button can be in.
    enum State {
        Struck = 0b00,  ///< Input went from low to high   (0,0)
        Idle = 0b11, ///< Input went from low to low (1,1)
        Playing = 0b10,  ///< Input went from high to high  (1,0)
        TurnOff = 0b01    ///< Input went from high to low  (0,1)
    };

    /**
     * @brief   Read the piezo and return its new state.
     *
     * The piezo has a minimum threshold for a hit. If the threshold 
     * is too low, it will go off randomly. Too high and it will never hit. 
     * It also has a  
     *
     *
     * @return  The state of the button, either Piezo::PRESSED,
     *          Piezo::RELEASED, Piezo::FALLING or Piezo::RISING.
     */
    State update();

    /**
     * @brief   Get the state of the button, without updating it.
     *          Returns the same value as the last call to @ref update.
     *
     * @return  The state of the button, either Piezo::PRESSED,
     *          Piezo::RELEASED, Piezo::FALLING or Piezo::RISING.
     */
    State getState() const;

    void dropState();

    // Get the velocity as a value between 1 and 127
    uint8_t getVelocity() const;

    // samples the background noise and returns the highest
    // level detected (0-1023)
    uint16_t sampleSilence(uint16_t sampleTime);

    // Sets the minimum threshold for a hit to be detected.
    // If this is too low, you will get random hits. Too high
    // and it will not detect hits. 
    void setHitThreshold(uint16_t hitThreshold);

    /// @brief   Return the name of the state as a string.
    static FlashString_t getName(State state);

  private:
    pin_t pin;

    uint16_t threshold = 10; // minimum input to make a hit
    bool recentlyHit = false;
    uint16_t highestYet;
    unsigned long highestInputTime = 0;
    bool struck = false; 
    State state = Idle;
    uint8_t velocity;
    uint16_t maxInput = MaxInputInitial;
    unsigned long startTime = 0;

    /// Edit this in Settings.hpp
    /// Changes how long a hit on the piezo lasts
    static unsigned long hitLength;
};

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
