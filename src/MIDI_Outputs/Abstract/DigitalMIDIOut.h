#pragma once

#include "../../Settings/SettingsWrapper.h"
#include "../MIDI_Control_Element.h"
/**
 * @brief
 * A class for MIDI_Control_Element%s that send MIDI notes.
 */
class DigitalMIDIOut : public MIDI_Control_Element {
  public:
    void setVelocity(uint8_t velocity);

  private:
    /**
     * @brief   Update the channel and address offset and refresh the subclass
     *          implementation.
     */
    void update() final override;
    /**
     * @brief   Refresh the underlying hardware and send a MIDI event if
     *          necessary.  
     *          To be implemented by subclasses.
     */
    virtual void refresh() = 0;
    /**
     * @brief   Check if any controls are active (i.e. it's not safe to
     *          update the channel and address offsets).
     *          To be implemented by subclasses.
     */
    virtual bool isActive() const = 0;

    void setChannelOffset(uint8_t offset) final override;
    void setAddressOffset(uint8_t offset) final override;

    uint8_t newAddressOffset = addressOffset;
    uint8_t newChannelOffset = channelOffset;
};