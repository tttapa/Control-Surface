#pragma once

#include "../../Settings/SettingsWrapper.h"
#include <MIDI_Outputs/MIDI_Output.hpp>

/**
 * @brief
 * A class for MIDI_Control_Element%s that send MIDI notes.
 */
class DigitalMIDIOut : public virtual MIDI_Output_Addressable {
  protected:
    /**
     * @brief   Send the appropriate MIDI event for turning on the button.
     */
    virtual void sendOn() const = 0;

    /**
     * @brief   Send the appropriate MIDI event for turning off the button.
     */
    virtual void sendOff() const = 0;
};