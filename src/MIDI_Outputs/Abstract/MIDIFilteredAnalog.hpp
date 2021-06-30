#pragma once

#include <AH/Hardware/FilteredAnalog.hpp>
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class for potentiometers and faders that send MIDI events.
 *
 * The analog input is filtered and hysteresis is applied.
 *
 * @see     FilteredAnalog
 */
template <class Sender>
class MIDIFilteredAnalog : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIFilteredAnalog.
     *
     * @param   analogPin
     *          The analog input pin with the wiper of the potentiometer
     *          connected.
     * @param   address
     *          The MIDI address to send to.
     * @param   sender
     *          The MIDI sender to use.
     */
    MIDIFilteredAnalog(pin_t analogPin, MIDIAddress address,
                       const Sender &sender)
        : filteredAnalog(analogPin), address(address), sender(sender) {}

  public:
    void begin() final override { filteredAnalog.resetToCurrentValue(); }

    void update() final override {
        if (filteredAnalog.update())
            forcedUpdate();
    }

    /// Send the value of the analog input over MIDI, even if the value didn't
    /// change.
    void forcedUpdate() { sender.send(filteredAnalog.getValue(), address); }

    /**
     * @brief   Specify a mapping function that is applied to the raw
     *          analog value before sending.
     *
     * @param   fn
     *          A function pointer to the mapping function. This function
     *          should take the filtered analog value of @f$ 16 - 
     *          \mathrm{ANALOG\_FILTER\_SHIFT\_FACTOR} @f$ bits as a parameter, 
     *          and should return a value in the same range.
     * 
     * @see     FilteredAnalog::map
     */
    void map(MappingFunction fn) { filteredAnalog.map(fn); }

    /// Invert the analog value.
    void invert() { filteredAnalog.invert(); }

    /**
     * @brief   Get the raw value of the analog input (this is the value 
     *          without applying the filter or the mapping function first).
     */
    analog_t getRawValue() const { return filteredAnalog.getRawValue(); }

    /**
     * @brief   Get the value of the analog input (this is the value after first
     *          applying the mapping function).
     */
    analog_t getValue() const { return filteredAnalog.getValue(); }

  private:
    AH::FilteredAnalog<Sender::precision()> filteredAnalog;
    MIDIAddress address;

  public:
    Sender sender;
};

END_CS_NAMESPACE