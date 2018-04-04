#ifndef DIGITAL_H
#define DIGITAL_H

#include "./MIDI_Control_Element.h"
#include "../Settings/SettingsWrapper.h"
#include "../ExtendedInputOutput/ExtendedInputOutput.h"
/**
 * @brief A class for MIDI_Control_Element%s that read from a digital input 
 * and send either MIDI Note events or MIDI Control Change events.
 * 
 * The input is debounced, the debounce time can be changed in Settings.h.
 */
class Digital : public MIDI_Control_Element
{
public:
  /**
   * @brief The message type for this element.
   * 
   */
  enum MessageType {
    /**
     * @brief Send MIDI Note On events when the button is pressed and Note Off events when the button is released.
     */
    NOTE = 0x90,
    /**
     * @brief Send MIDI Control Change events with a value of \c velocity when the button is pressed and Control Change events with a value of 0 when the button is released.
     */
    CC = 0xB0
  };
  /**
   * @brief Construct a new Digital object.
   * 
   * @param pin The digital pin to read from. The internal pull-up resistor will be enabled.
   * @param address The MIDI Note number (\c messageType == NOTE) [0, 127] or MIDI Controller number (\c messageType == CC) [0, 119].
   * @param channel The MIDI channel. [1, 16]
   * @param velocity The MIDI Note velocity (\c messageType == NOTE) [1, 127] or Controller value when on (\c messageType == CC) [1, 127].
   * @param messageType Sets the MIDI message type. Either Digital::NOTE or Digital::CC.
   */
  Digital(pin_t pin, uint8_t address, uint8_t channel, uint8_t velocity = 127, MessageType messageType = NOTE); // Constructor
  ~Digital();                                                                // Destructor
  /**
   * @brief Invert the state of all buttons (button pressed is HIGH instead of LOW).
   * 
   * @note This impacts **all** Digital objects.
   */
  static void invert();

private:
  void refresh(); // Check if the button state changed, and send a MIDI Note On or Off accordingly

  const pin_t pin;
  const uint8_t address;
  const uint8_t channel;
  const uint8_t velocity;
  const MessageType messageType;

  bool prevState = HIGH, buttonState = HIGH;
  unsigned long prevBounceTime = 0;

  static bool invertState;

  const static unsigned long debounceTime = BUTTON_DEBOUNCE_TIME;  // Edit this in ../Settings/Settings.h

  const static int8_t falling = LOW - HIGH;
  const static int8_t rising = HIGH - LOW;

  void setChannelOffset(uint8_t offset);
  void setAddressOffset(uint8_t offset);

  uint8_t newAddressOffset = addressOffset;
  uint8_t newChannelOffset = channelOffset;
};

#endif // DIGITAL_H
