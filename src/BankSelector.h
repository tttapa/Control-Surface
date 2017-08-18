#include "Arduino.h"
#include "Bank.h"
#ifdef __AVR__
#include "initializer_list.h"
#else
#include <initializer_list>
#endif

#define EXT_PIN(x) (x + NUM_DIGITAL_PINS)

class BankSelector
{
public:
  enum ButtonType
  {
    TOGGLE,
    MOMENTARY
  };

  /*
  Different Bank Select modes:

  - One toggle switch (latching switch)

      When the switch is in the 'off' position, channel 1 is selected
      When the switch is in the 'on' position, channel 2 is selected

      BankSelector(bank, switch pin, BankSelector::TOGGLE);

  - One toggle switch (latching switch) and one LED

      When the switch is in the 'off' position, channel 1 is selected and the LED is off
      When the switch is in the 'on' position, channel 2 is selected and the LED is on

      BankSelector(bank, switch pin, led pin, BankSelector::TOGGLE);

  - One momentary switch (push button)

      Pressing the button switches the channel:
      When starting the program, channel 1 is selected,
      When the button is pressed, channel 2 is selected,
      When the button is pressed again, channel 1 is selected, 
      and so on.

      BankSelector(bank, button pin);
      BankSelector(bank, button pin, BankSelector::MOMENTARY);

  - One momentary switch (push button) and one LED

      Pressing the button switches the channel and toggles the LED:
      When starting the program, channel 1 is selected and the LED is off,
      When the button is pressed, channel 2 is selected and the LED turns on,
      When the button is pressed again, channel 1 is selected and the LED is turned off,
      and so on.

      BankSelector(bank, button pin, led pin);
      BankSelector(bank, button pin, led pin, BankSelector::MOMENTARY);

  - Multiple momentary switches (push buttons)

      Pressing one of the buttons selects the respective output
      When starting the program, channel 1 is selected,
      When the second button is pressed, channel 2 is selected,
      When the n-th button is pressed, channel n is selected.

      BankSelector(bank, { button 1 pin, button 2 pin, ... , button n pin } );


    Note: a switch is 'off' or 'released' when it doesn't conduct. The digital value 
    on the input will therefore be HIGH (because of the pull-up resistor)
  */

  BankSelector(Bank &bank, uint8_t switchPin, ButtonType buttonType = MOMENTARY) // One switch or button, no LEDs
      : bank(bank), switchPin(switchPin)
  {
    mode = buttonType == TOGGLE ? SINGLE_SWITCH : SINGLE_BUTTON;
  }
  BankSelector(Bank &bank, uint8_t switchPin, uint8_t ledPin, ButtonType buttonType = MOMENTARY) // One switch or button, one LED
      : bank(bank), switchPin(switchPin), ledPin(ledPin)
  {
    mode = buttonType == TOGGLE ? SINGLE_SWITCH_LED : SINGLE_BUTTON_LED;
  }
  template <size_t N>
  BankSelector(Bank &bank, const uint8_t (&switchPins)[N]) // Multiple buttons, no LEDs
      : bank(bank), switchPins(switchPins), nb_banks(N)
  {
    mode = MULTIPLE_BUTTONS;
  }
  BankSelector(Bank &bank, std::initializer_list<uint8_t> switchPins) // Multiple buttons, no LEDs
      : bank(bank), switchPins(switchPins.begin()), nb_banks(switchPins.size())
  {
    mode = MULTIPLE_BUTTONS;
  }
  template <size_t M, size_t N>
  BankSelector(Bank &bank, const uint8_t (&switchPins)[M], const uint8_t (&ledPins)[N]) // Multiple buttons, multiple LEDs
      : bank(bank), switchPins(switchPins), ledPins(ledPins), nb_banks(N)
  {
    if (M == 1)
    {
      mode = INCREMENT_LEDS;
    }
    else if (M == 2)
    {
      mode = INCREMENT_DECREMENT_LEDS;
    }
    else
    {
      mode = MULTIPLE_BUTTONS_LEDS;
      nb_banks = N < M ? N : M;
    }
  }
  BankSelector(Bank &bank, std::initializer_list<uint8_t> switchPins, std::initializer_list<uint8_t> ledPins) // Multiple buttons, multiple LEDs
      : bank(bank), switchPins(switchPins.begin()), ledPins(ledPins.begin()), nb_banks(ledPins.size())
  {
    if (switchPins.size() == 1)
    {
      mode = INCREMENT_LEDS;
    }
    else if (switchPins.size() == 2)
    {
      mode = INCREMENT_DECREMENT_LEDS;
    }
    else
    {
      mode = MULTIPLE_BUTTONS_LEDS;
      nb_banks = switchPins.size() < ledPins.size() ? switchPins.size() : ledPins.size();
    }
  }
  BankSelector(Bank &bank, const uint8_t (&switchPins)[2], uint8_t nb_banks) // Two buttons (+1, -1), no LEDs
      : bank(bank), switchPins(switchPins), nb_banks(nb_banks)
  {
    mode = INCREMENT_DECREMENT;
  }
  BankSelector(Bank &bank, const uint8_t (&switchPins)[1], uint8_t nb_banks) // One button (+1), no LEDs
      : bank(bank), switchPins(switchPins), nb_banks(nb_banks)
  {
    mode = INCREMENT;
  }
  BankSelector(Bank &bank, std::initializer_list<uint8_t> switchPins, uint8_t nb_banks) // Two buttons (+1, -1), no LEDs
      : bank(bank), switchPins(switchPins.begin()), nb_banks(nb_banks)
  {
    mode = switchPins.size() > 1 ? INCREMENT_DECREMENT : INCREMENT;
  }

  void init();
  void refresh();

  uint8_t getChannel();
  void setChannel(uint8_t newChannel);

  const char *getMode();

  void setPinModeExt(void (*fn)(uint8_t, uint8_t));
  void setDigitalWriteExt(void (*fn)(uint8_t, uint8_t));
  void setDigitalReadExt(int (*fn)(uint8_t));
  void setChannelChangeEvent(void (*fn)(uint8_t));

private:
  Bank &bank;
  uint8_t channel = 1;
  uint8_t switchPin, ledPin;
  const uint8_t *switchPins, *ledPins;
  uint8_t nb_banks;

  unsigned long prevBounceTime = 0;
  bool prevIncrementState = HIGH;
  bool prevDecrementState = HIGH;

  bool prevToggleState = HIGH;

  const unsigned long debounceTime = 25;

  const int8_t falling = LOW - HIGH;
  const int8_t rising = HIGH - LOW;

  void (*pinModeExt)(uint8_t, uint8_t) = nullptr;
  void (*digitalWriteExt)(uint8_t, uint8_t) = nullptr;
  int (*digitalReadExt)(uint8_t) = nullptr;

  void (*channelChangeEvent)(uint8_t) = nullptr;

  enum BankSelectorMode
  {
    SINGLE_BUTTON,
    SINGLE_BUTTON_LED,
    SINGLE_SWITCH,
    SINGLE_SWITCH_LED,
    MULTIPLE_BUTTONS,
    MULTIPLE_BUTTONS_LEDS,
    INCREMENT_DECREMENT,
    INCREMENT_DECREMENT_LEDS,
    INCREMENT,
    INCREMENT_LEDS
  } mode;

  void refreshLEDs(uint8_t newChannel);

  void pinMode(uint8_t pin, uint8_t mode);
  void digitalWrite(uint8_t pin, uint8_t val);
  int digitalRead(uint8_t pin);
};