#pragma once

#include "../../Hardware/FilteredAnalog.h"
#include "../MIDI_Control_Element.h"

/**
 * @brief   A class for potentiometers and faders that send MIDI events.
 * 
 * The analog input is filtered and hysteresis is applied.
 * 
 * @tparam  PRECISION
 *          The analog precision in bits.
 * 
 * @see     FilteredAnalog
 */
template <uint8_t PRECISION>
class MIDIFilteredAnalog : public MIDI_Control_Element {
  public:
    /**
     * @brief   Construct a new MIDIFilteredAnalog.
     * 
     * @param   analogPin
     *          The analog input pin with the wiper of the potentiometer
     *          connected.
     */
    MIDIFilteredAnalog(pin_t analogPin) : filteredAnalog{analogPin} {}

  private:
    /**
     * @brief   Send the given value as a MIDI event.
     * 
     * @param   value
     *          The value to be sent over MIDI.  
     *          It is `PRECISION` bits wide, so a number between 0 and 
     *          2<sup>`PRECISION`</sup> - 1.
     */
    virtual void send(uint16_t value) const = 0;
    
    void update() final override {
        if (filteredAnalog.update())
            send(filteredAnalog.getValue());
    }

    FilteredAnalog<PRECISION> filteredAnalog;
};