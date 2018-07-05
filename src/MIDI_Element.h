#pragma once

#include <stdint.h> // uint8_t

#include "./Settings/SettingsWrapper.h"

/**
 * @brief   The abstract class that all MIDI elements (both inputs and outputs)
 *          inherit from.  
 *          It declares methods for updating these elements, resetting them,
 *          and changing settings related to banks (channel/address offset
 *          and number of tracks per bank).
 */
class MIDI_Element {
    friend class Control_Surface_;

  protected:
    /**
     * @brief Reset to default value (currently only used for VU meters)
     */
    virtual void reset() {}
    /**
     * @brief Check if the state or value of the MIDI Element has changed since
     *        last update, if so, send a MIDI event or update
     *        the MIDI_Input_Element (write buffer to output etc.)
     */
    virtual void update() = 0;

  public:
    /**
     * @see FilteredAnalog::map
     */
    virtual void map(int (*fn)(int)) {}

    /**
     * @brief Set the channel offset.
     *
     * @param offset
     */
    virtual void setChannelOffset(uint8_t offset) { channelOffset = offset; }
    /**
     * @brief Set the address (note or controller number) offset
     *
     * @param offset
     */
    virtual void setAddressOffset(uint8_t offset) { addressOffset = offset; }
    /**
     * @brief Set the number of tracks per bank
     *
     * @param tracks
     */
    virtual void settracksPerBank(uint8_t tracks) { tracksPerBank = tracks; }

  protected:
    uint8_t channelOffset = 0;
    uint8_t addressOffset = 0;
    uint8_t tracksPerBank = 1;
};