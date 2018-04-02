#ifndef ANALOG_h_
#define ANALOG_h_

// #include <Arduino.h>
#include "../Settings/SettingsWrapper.h"
#include "../Helpers/EMA.h"
#include "./MIDI_Control_Element.h"
#include "../ExtendedInputOutput/ExtendedInputOutput.h"

/**
 * A pure virtual class for MIDI_Control_Element%s that read from an analog input.
 * 
 * A map function can be applied to the analog value (e.g. to compensate for logarithmic taper potentiometers or to calibrate the range).  
 * The analog input value is filtered using an exponential moving average filter. The settings for this filter can be changed in Settings.h.  
 * No specific function for sending the value as a MIDI message is implemented. Classes that inherit from this class should implement the send function.  
 * 
 * Just like any MIDI_Control_Element, it will be added to the list of elements, and it will be automatically refreshed by the Control Surface.  
 */
class AnalogBase : public MIDI_Control_Element
{
public:
  /**
   * @brief Construct a new AnalogBase object.
   * 
   * @param analogPin 
   *        The analog pin to read from.
   */
  AnalogBase(pin_t analogPin); // Constructor
  /**
   * @brief Specify a mapping function that is applied to the raw analog value before sending.
   * 
   * @param fn 
   *        A function pointer to the mapping function. This function should take the 10-bit analog value as a parameter, and should return a 10-bit value.
   */
  void map(int (*fn)(int));

private:
  /**
   * @brief Read the analog input value, apply the mapping function, and update the average.
   */
  void refresh();

  pin_t analogPin;
  uint8_t controllerNumber, channel;
  int (*mapFn)(int) = nullptr;
  EMA<ANALOG_FILTER_SHIFT_FACTOR, int32_t> filter;

protected:
  /**
 * @brief The pure virtual function that actually sends the analog value over MIDI.
 * 
 * @param value
 *        The 10-bit value to be sent over MIDI.
 */
  virtual void send(unsigned int value) = 0;
};

#define Analog _Pragma("GCC warning \"'Analog' is deprecated\"") AnalogCC

/**
 * @brief A MIDI_Control_Element that reads an analog input and sends it over MIDI as a 7-bit Control Change event. 
 */
class AnalogCC : public AnalogBase
{
public:
  /**
   * @brief Construct a new AnalogCC object.
   * 
   * @param analogPin 
   *        The analog pin to read from.
   * @param controller 
   *        The MIDI Controller number [0, 119].
   * @param channel
   *        The MIDI channel [1, 16].
   */
  AnalogCC(pin_t analogPin, uint8_t controller, uint8_t channel)
      : AnalogBase(analogPin), controller(controller), channel(channel) {}

protected:
  /**
   * @brief Send the analog value over MIDI as a 7-bit Control Change event.
   * 
   * Only sends a MIDI message if the value has changed.
   * 
   * @param value 
   *        The 10-bit analog value to be sent.
   */
  virtual void send(unsigned int value);
  uint8_t oldVal = -1;

  uint8_t controller, channel;
};

#define AnalogHiRes _Pragma("GCC warning \"'AnalogHiRes' is deprecated\"") AnalogPB

/**
 * @brief A MIDI_Control_Element that reads an analog input and sends it over MIDI as a 14-bit Pitch Bend event. 
 */
class AnalogPB : public AnalogBase
{
public:
  /**
   * @brief Construct a new AnalogPB object.
   * 
   * @param analogPin 
   *        The analog pin to read from.
   * @param channel
   *        The MIDI channel [1, 16].
   */
  AnalogPB(pin_t analogPin, uint8_t channel)
      : AnalogBase(analogPin), channel(channel) {}

protected:
  /**
   * @brief Send the analog value over MIDI as a 14-bit Pitch Bend event.
   * 
   * Only sends a MIDI message if the value has changed.
   * 
   * @param value 
   *        The 10-bit analog value to be sent.
   */
  virtual void send(unsigned int value);
  uint16_t oldVal = -1;

  uint8_t channel;
};

#endif // ANALOG_h_
