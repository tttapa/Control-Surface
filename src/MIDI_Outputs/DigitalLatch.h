#ifndef DIGLAT_h_
#define DIGLAT_h_

#include "./MIDI_Control_Element.h"
#include "../ExtendedInputOutput/ExtendedInputOutput.h"

/**
 * @brief A class for MIDI_Control_Element%s that read from a digital input
 * and send a MIDI Note On event followed by a Note Off event 
 * \c latchTime milliseconds later, on each state change of the input.  
 * It is meant for use with toggle (latching) switches.
 * 
 * The input is debounced, the debounce time can be changed in Settings.h.
 */
class DigitalLatch : public MIDI_Control_Element
{
public:
/**
 * @brief Construct a new Digital Latch object.
 * 
 * @param pin pin The digital pin to read from. The internal pull-up resistor will be enabled.
 * @param note The MIDI Note number. [0, 127]
 * @param channel The MIDI channel. [1, 16] 
 * @param velocity The velocity of the MIDI Note On and Note Off events.
 * @param latchTime The delay (in milliseconds) between the Note On and Note Off events.
 */
  DigitalLatch(pin_t pin, uint8_t note, uint8_t channel, uint8_t velocity = 0x7F, unsigned long latchTime = 100); // Constructor
  ~DigitalLatch();                                                                                                // Destructor

private:
  void update(); // Check if the button state changed, if so, send a MIDI Note On, after a non-blocking delay of "latchTime", send a Note Off

  const pin_t pin;
  const uint8_t note;
  const uint8_t channel;
  const uint8_t velocity;

  bool oldState = HIGH;
  bool noteOffSent = true;
  unsigned long latchTime;
  unsigned long noteOnTime;

  void setChannelOffset(uint8_t offset);
  void setAddressOffset(uint8_t offset);

  uint8_t newAddressOffset = addressOffset;
  uint8_t newChannelOffset = channelOffset;
};

#endif
