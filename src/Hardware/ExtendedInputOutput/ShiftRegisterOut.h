#pragma once

#include "ExtendedIOElement.h"

/**
 * @brief   A class for serial-in/parallel-out shift registers, 
 *          like the 74HC595. 
 */
class ShiftRegisterOut : public ExtendedIOElement {
  public:
    /**
     * @brief   Create a new ShiftRegisterOut object with a shift register
     *          connected to the given pins, with a given bit order,
     *          and a given number of outputs.
     * 
     * Multiple shift registers can be cascaded:
     * ```
     * clockPin >──────────────┬─────────────────────────────┬─────────────────────────────┐            
     *              ┎━━━━━━━━━━┷━━━━━━━━━━━┓      ┎━━━━━━━━━━┷━━━━━━━━━━━┓      ┎━━━━━━━━━━┷━━━━━━━━━━━┓
     *              ┃        SH_CP         ┃      ┃        SH_CP         ┃      ┃        SH_CP         ┃
     * dataPin  >───┨ Data in     Data out ┠──────┨ Data in     Data out ┠──────┨ Data in     Data out ┃
     *              ┃        ST_CP         ┃      ┃        ST_CP         ┃      ┃        ST_CP         ┃
     *              ┗━━━━━━━━━━┯━━━━━━━━━━━┛      ┗━━━━━━━━━━┯━━━━━━━━━━━┛      ┗━━━━━━━━━━┯━━━━━━━━━━━┛
     * latchPin >──────────────┴─────────────────────────────┴─────────────────────────────┘            
     * ```
     * 
     * @param   dataPin
     *          The digital output pin connected to the serial data input (DS)
     *          of the shift register.
     * @param   clockPin
     *          The digital output pin connected to the clock input (SH_CP)
     *          of the shift register.
     * @param   latchPin
     *          The digital output pin connected to the latch pin (ST_CP)
     *          of the shift register.
     */
    ShiftRegisterOut(pin_t dataPin, pin_t clockPin, pin_t latchPin,
                     uint8_t bitOrder, pin_t length = 8);
    /** 
     * @brief   Destructor.
     */
    ~ShiftRegisterOut();

    /**
     * @brief   The pinMode function is not implemented because the mode is
     *          `OUTPUT` by definition.
     */
    void pinMode(pin_t pin, uint8_t mode) __attribute__ ((deprecated)) {}
    /**
     * @brief   Set the state of a given output pin.
     * 
     * @param   pin
     *          The shift register pin to set.
     * @param   val
     *          The value to set the pin to.
     *          (Either `HIGH` (1) or `LOW` (0))
     */
    void digitalWrite(pin_t pin, uint8_t val);
    /**
     * @brief   Get the current state of a given output pin.
     * 
     * @param   pin
     *          The pin to read.
     */
    int digitalRead(pin_t pin);
    /**
     * @brief   The analogRead function is not implemented because a shift
     *          is always digital.
     */
    analog_t analogRead(pin_t pin) __attribute__ ((deprecated)) {}
    /**
     * @brief   The analogWrite function is not implemented because a shift
     *          is always digital.
     */
    void analogWrite(pin_t pin, analog_t val) __attribute__ ((deprecated)) {}

    /**
     * @brief   Initialize the shift register.  
     *          Set the data and clock pins to output mode, 
     *          and set all shift register outputs to `LOW`.
     */
    void begin();
    /**
     * @brief   Write the state buffer to the physical outputs.
     */
    void update();
    /**
     * @brief   Set all shift register outputs to `LOW`.
     */
    void reset();

    /**
     * @brief   Get the red output pin of the given LED.
     * 
     * @param   id
     *          The zero-based LED number.
     */
    pin_t red(pin_t id);
    /**
     * @brief   Get the green output pin of the given LED.
     * 
     * @param   id
     *          The zero-based LED number.
     */
    pin_t green(pin_t id);
    /**
     * @brief   Get the blue output pin of the given LED.
     * 
     * @param   id
     *          The zero-based LED number.
     */
    pin_t blue(pin_t id);

    /**
     * @brief   The position of the red output pin for 3-color LEDs.  
     *          For the usual RGB configuration, this is 0.
     */
    uint8_t redBit = 0;
    /**
     * @brief   The position of the green output pin for 3-color LEDs.  
     *          For the usual RGB configuration, this is 1.
     */
    uint8_t greenBit = 1;
    /**
     * @brief   The position of the blue output pin for 3-color LEDs.  
     *          For the usual RGB configuration, this is 2.
     */
    uint8_t blueBit = 2;

  private:
    pin_t dataPin, clockPin, latchPin;
    uint8_t bitOrder, bufferLength;

    uint8_t *stateBuffer = nullptr;

    int8_t pinToBufferIndex(pin_t pin);
    uint8_t pinToBitMask(pin_t pin);
    constexpr static int8_t INVALID_PIN = -1;
};