#ifndef BANKSELECTOR_H_
#define BANKSELECTOR_H_

#include "Arduino.h"
#include "Bank.h"
#include "ExtendedInputOutput/ExtendedInputOutput.h"

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

          Note: this mode is pretty useless, you can just connect the LED to the switch directly,
                without wasting a digital output pin on it.

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

      Pressing one of the buttons selects the respective output:
      When starting the program, channel 1 is selected,
      When the second button is pressed, channel 2 is selected,
      When the n-th button is pressed, channel n is selected.

      BankSelector(bank, { button 1 pin, button 2 pin, ... , button n pin } );


  - Multiple momentary switches (push buttons) and multiple LEDs

      Pressing one of the buttons selects the respective output and enables the respective LED:
      When starting the program, channel 1 is selected and LED 1 is on,
      When the second button is pressed, channel 2 is selected, LED 1 turns off and LED 2 turns on,
      When the n-th button is pressed, channel n is selected, LED n turns on, and all other LEDs are off.

      BankSelector(bank, { button 1 pin, button 2 pin, ... , button n pin }, { led 1 pin, led 2 pin, ... , led n pin } );

  - Two momentary switches (push buttons)

      Pressing the first button increments the channel number,
      pressing the second button decrements the channel number: 
      When starting the program, channel 1 is selected,
      When the first button is pressed, channel 2 is selected, 
      When the first button is pressed again, channel 3 is selected,
      When the last channel is selected, and the first button is pressed again,
      channel 1 is selected.
      When the second button is pressed, the last channel (n) is selected,
      When the second button is pressed again, channel (n-1) is selected,
      and so on.

      BankSelector(bank, { button increment pin, button decrement pin }, number of channels);


  - Two momentary switches (push buttons) and multiple LEDs

      Pressing the first button increments the channel number and turns on the respective LED,
      pressing the second button decrements the channel number and turns on the respective LED: 
      When starting the program, channel 1 is selected and LED 1 is on,
      When the first button is pressed, channel 2 is selected, LED 1 turns off and LED 2 turns on,
      When the first button is pressed again, channel 3 is selected, LED 2 turns off and LED 3 turns on.
      When the last channel is selected, and the first button is pressed,
      channel 1 is selected, the last LED turns off and LED 1 turns on.
      When the second button is pressed, the last channel (n) is selected, LED 1 turns off and LED n turns on,
      When the second button is pressed again, channel (n-1) is selected, LED n turns off and LED n-1 turns on,
      and so on.

      BankSelector(bank, { button increment pin, button decrement pin }, { led 1 pin, led 2 pin, ... , led n pin });


  - One momentary switch (push button)

      Pressing the button increments the channel number,
      When starting the program, channel 1 is selected,
      When the button is pressed, channel 2 is selected, 
      When the button is pressed again, channel 3 is selected,
      When the last channel is selected, and the button is pressed again,
      channel 1 is selected.

      BankSelector(bank, { button increment pin }, number of channels);


  - One momentary switch (push button) and multiple LEDs

      Pressing the button increments the channel number and turns on the respective LED,
      When starting the program, channel 1 is selected and LED 1 is on,
      When the button is pressed, channel 2 is selected, LED 1 turns off and LED 2 turns on,
      When the button is pressed again, channel 3 is selected, LED 2 turns off and LED 3 turns on.
      When the last channel is selected, and the button is pressed,
      channel 1 is selected, the last LED turns off and LED 1 turns on.
      
      BankSelector(bank, { button increment pin }, { led 1 pin, led 2 pin, ... , led n pin });


    Note: a switch is 'off' or 'released' when it doesn't conduct. The digital value 
    on the input will therefore be HIGH (because of the pull-up resistor)
  */

  BankSelector(Bank &bank, pin_t switchPin, ButtonType buttonType = MOMENTARY) // One switch or button, no LEDs
      : bank(bank), switchPin(switchPin)
  {
    mode = buttonType == TOGGLE ? SINGLE_SWITCH : SINGLE_BUTTON;
  }
  BankSelector(Bank &bank, pin_t switchPin, pin_t ledPin, ButtonType buttonType = MOMENTARY) // One switch or button, one LED
      : bank(bank), switchPin(switchPin), ledPin(ledPin)
  {
    mode = buttonType == TOGGLE ? SINGLE_SWITCH_LED : SINGLE_BUTTON_LED;
  }
  template <size_t N>
  BankSelector(Bank &bank, const pin_t (&switchPins)[N]) // Multiple buttons, no LEDs
      : bank(bank), switchPins(switchPins), nb_banks(N)
  {
    mode = MULTIPLE_BUTTONS;
  }
  BankSelector(Bank &bank, std::initializer_list<pin_t> switchPins) // Multiple buttons, no LEDs
      : bank(bank), nb_banks(switchPins.size())
  {
    switchPinsStorage = (pin_t *)malloc(sizeof(pin_t) * switchPins.size());
    memcpy(switchPinsStorage, switchPins.begin(), sizeof(pin_t) * switchPins.size());
    this->switchPins = switchPinsStorage;
    mode = MULTIPLE_BUTTONS;
  }
  template <size_t M, size_t N>
  BankSelector(Bank &bank, const pin_t (&switchPins)[M], const pin_t (&ledPins)[N]) // One or multiple buttons, multiple LEDs
      : bank(bank), switchPins(switchPins), ledPins(ledPins), nb_banks(N)
  {
    if (M == 1)
      mode = INCREMENT_LEDS;
    else if (M == 2)
      mode = INCREMENT_DECREMENT_LEDS;
    else
    {
      mode = MULTIPLE_BUTTONS_LEDS;
      nb_banks = N < M ? N : M;
    }
  }
  BankSelector(Bank &bank, std::initializer_list<pin_t> switchPins, std::initializer_list<pin_t> ledPins) // One or multiple buttons, multiple LEDs
      : bank(bank), nb_banks(ledPins.size())
  {
    switchPinsStorage = (pin_t *)malloc(sizeof(pin_t) * switchPins.size());
    memcpy(switchPinsStorage, switchPins.begin(), sizeof(pin_t) * switchPins.size());
    this->switchPins = switchPinsStorage;
    ledPinsStorage = (pin_t *)malloc(sizeof(pin_t) * ledPins.size());
    memcpy(ledPinsStorage, ledPins.begin(), sizeof(pin_t) * ledPins.size());
    this->ledPins = ledPinsStorage;

    if (switchPins.size() == 1)
      mode = INCREMENT_LEDS;
    else if (switchPins.size() == 2)
      mode = INCREMENT_DECREMENT_LEDS;
    else
    {
      mode = MULTIPLE_BUTTONS_LEDS;
      nb_banks = switchPins.size() < ledPins.size() ? switchPins.size() : ledPins.size();
    }
  }
  BankSelector(Bank &bank, const pin_t (&switchPins)[2], pin_t nb_banks) // Two buttons (+1, -1), no LEDs
      : bank(bank), switchPins(switchPins), nb_banks(nb_banks)
  {
    mode = INCREMENT_DECREMENT;
  }
  BankSelector(Bank &bank, const pin_t (&switchPins)[1], pin_t nb_banks) // One button (+1), no LEDs
      : bank(bank), switchPins(switchPins), nb_banks(nb_banks)
  {
    mode = INCREMENT;
  }
  BankSelector(Bank &bank, std::initializer_list<pin_t> switchPins, pin_t nb_banks) // One or two buttons (+1, (-1)), no LEDs
      : bank(bank), nb_banks(nb_banks)
  {
    switchPinsStorage = (pin_t *)malloc(sizeof(pin_t) * switchPins.size());
    memcpy(switchPinsStorage, switchPins.begin(), sizeof(pin_t) * switchPins.size());
    this->switchPins = switchPinsStorage;
    mode = switchPins.size() > 1 ? INCREMENT_DECREMENT : INCREMENT;
  }

  ~BankSelector()
  {
    free(ledPinsStorage);
    free(switchPinsStorage);
  }

  void init();
  void refresh();

  uint8_t getChannel();
  void setChannel(uint8_t newChannel);

  const char *getMode();

  void setChannelChangeEvent(void (*fn)(uint8_t));

private:
  Bank &bank;
  uint8_t channel = 1;
  pin_t switchPin, ledPin;
  const pin_t *switchPins, *ledPins;
  pin_t *switchPinsStorage = nullptr, *ledPinsStorage = nullptr;
  uint8_t nb_banks;

  unsigned long prevBounceTime = 0;
  bool prevIncrementState = HIGH;
  bool prevDecrementState = HIGH;

  bool prevToggleState = HIGH;

  const unsigned long debounceTime = 25;

  const int8_t falling = LOW - HIGH;
  const int8_t rising = HIGH - LOW;

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
};

#endif // BANKSELECTOR_H_