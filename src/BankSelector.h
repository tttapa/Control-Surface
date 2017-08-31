#ifndef BANKSELECTOR_H_
#define BANKSELECTOR_H_

#include "Arduino.h"
#include "Bank.h"
#include "Linked_List.h"
#include "ExtendedInputOutput/ExtendedInputOutput.h"

#ifdef __AVR__
#include "initializer_list.h"
#else
#include <initializer_list>
#endif

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

      When the switch is in the 'off' position, bankSetting 1 is selected
      When the switch is in the 'on' position, bankSetting 2 is selected

      BankSelector(bank, switch pin, BankSelector::TOGGLE);


  - One toggle switch (latching switch) and one LED

      When the switch is in the 'off' position, bankSetting 1 is selected and the LED is off
      When the switch is in the 'on' position, bankSetting 2 is selected and the LED is on

          Note: this mode is pretty useless, you can just connect the LED to the switch directly,
                without wasting a digital output pin on it.

      BankSelector(bank, switch pin, led pin, BankSelector::TOGGLE);


  - One momentary switch (push button)

      Pressing the button switches the bankSetting:
      When starting the program, bankSetting 1 is selected,
      When the button is pressed, bankSetting 2 is selected,
      When the button is pressed again, bankSetting 1 is selected, 
      and so on.

      BankSelector(bank, button pin);
      BankSelector(bank, button pin, BankSelector::MOMENTARY);


  - One momentary switch (push button) and one LED

      Pressing the button switches the bankSetting and toggles the LED:
      When starting the program, bankSetting 1 is selected and the LED is off,
      When the button is pressed, bankSetting 2 is selected and the LED turns on,
      When the button is pressed again, bankSetting 1 is selected and the LED is turned off,
      and so on.

      BankSelector(bank, button pin, led pin);
      BankSelector(bank, button pin, led pin, BankSelector::MOMENTARY);


  - Multiple momentary switches (push buttons)

      Pressing one of the buttons selects the respective output:
      When starting the program, bankSetting 1 is selected,
      When the second button is pressed, bankSetting 2 is selected,
      When the n-th button is pressed, bankSetting n is selected.

      BankSelector(bank, { button 1 pin, button 2 pin, ... , button n pin } );


  - Multiple momentary switches (push buttons) and multiple LEDs

      Pressing one of the buttons selects the respective output and enables the respective LED:
      When starting the program, bankSetting 1 is selected and LED 1 is on,
      When the second button is pressed, bankSetting 2 is selected, LED 1 turns off and LED 2 turns on,
      When the n-th button is pressed, bankSetting n is selected, LED n turns on, and all other LEDs are off.

      BankSelector(bank, { button 1 pin, button 2 pin, ... , button n pin }, { led 1 pin, led 2 pin, ... , led n pin } );

  - Two momentary switches (push buttons)

      Pressing the first button increments the bankSetting number,
      pressing the second button decrements the bankSetting number: 
      When starting the program, bankSetting 1 is selected,
      When the first button is pressed, bankSetting 2 is selected, 
      When the first button is pressed again, bankSetting 3 is selected,
      When the last bankSetting is selected, and the first button is pressed again,
      bankSetting 1 is selected.
      When the second button is pressed, the last bankSetting (n) is selected,
      When the second button is pressed again, bankSetting (n-1) is selected,
      and so on.

      BankSelector(bank, { button increment pin, button decrement pin }, number of bankSettings);


  - Two momentary switches (push buttons) and multiple LEDs

      Pressing the first button increments the bankSetting number and turns on the respective LED,
      pressing the second button decrements the bankSetting number and turns on the respective LED: 
      When starting the program, bankSetting 1 is selected and LED 1 is on,
      When the first button is pressed, bankSetting 2 is selected, LED 1 turns off and LED 2 turns on,
      When the first button is pressed again, bankSetting 3 is selected, LED 2 turns off and LED 3 turns on.
      When the last bankSetting is selected, and the first button is pressed,
      bankSetting 1 is selected, the last LED turns off and LED 1 turns on.
      When the second button is pressed, the last bankSetting (n) is selected, LED 1 turns off and LED n turns on,
      When the second button is pressed again, bankSetting (n-1) is selected, LED n turns off and LED n-1 turns on,
      and so on.

      BankSelector(bank, { button increment pin, button decrement pin }, { led 1 pin, led 2 pin, ... , led n pin });


  - One momentary switch (push button)

      Pressing the button increments the bankSetting number,
      When starting the program, bankSetting 1 is selected,
      When the button is pressed, bankSetting 2 is selected, 
      When the button is pressed again, bankSetting 3 is selected,
      When the last bankSetting is selected, and the button is pressed again,
      bankSetting 1 is selected.

      BankSelector(bank, { button increment pin }, number of bankSettings);


  - One momentary switch (push button) and multiple LEDs

      Pressing the button increments the bankSetting number and turns on the respective LED,
      When starting the program, bankSetting 1 is selected and LED 1 is on,
      When the button is pressed, bankSetting 2 is selected, LED 1 turns off and LED 2 turns on,
      When the button is pressed again, bankSetting 3 is selected, LED 2 turns off and LED 3 turns on.
      When the last bankSetting is selected, and the button is pressed,
      bankSetting 1 is selected, the last LED turns off and LED 1 turns on.
      
      BankSelector(bank, { button increment pin }, { led 1 pin, led 2 pin, ... , led n pin });


    Note: a switch is 'off' or 'released' when it doesn't conduct. The digital value 
    on the input will therefore be HIGH (because of the pull-up resistor)
  */

  BankSelector(Bank &bank, pin_t switchPin, ButtonType buttonType = MOMENTARY) // One switch or button, no LEDs
      : bank(bank), switchPin(switchPin)
  {
    if (buttonType == TOGGLE)
    {
      mode = SINGLE_SWITCH;
    }
    else
    {
      dbButton1.pin = switchPin;
      mode = SINGLE_BUTTON;
    }
    init();
    INSERT_INTO_LINKED_LIST(this, first, last);
  }
  BankSelector(Bank &bank, pin_t switchPin, pin_t ledPin, ButtonType buttonType = MOMENTARY) // One switch or button, one LED
      : bank(bank), switchPin(switchPin), ledPin(ledPin)
  {
    if (buttonType == TOGGLE)
    {
      mode = SINGLE_SWITCH_LED;
    }
    else
    {
      dbButton1.pin = switchPin;
      mode = SINGLE_BUTTON_LED;
    }
    init();
    INSERT_INTO_LINKED_LIST(this, first, last);
  }
  template <size_t N>
  BankSelector(Bank &bank, const pin_t (&switchPins)[N]) // Multiple buttons, no LEDs
      : bank(bank), switchPins(switchPins), nb_settings(N)
  {
    mode = MULTIPLE_BUTTONS;
    init();
    INSERT_INTO_LINKED_LIST(this, first, last);
  }
  BankSelector(Bank &bank, std::initializer_list<pin_t> switchPins) // Multiple buttons, no LEDs
      : bank(bank), nb_settings(switchPins.size())
  {
    switchPinsStorage = (pin_t *)malloc(sizeof(pin_t) * switchPins.size());
    memcpy(switchPinsStorage, switchPins.begin(), sizeof(pin_t) * switchPins.size());
    this->switchPins = switchPinsStorage;
    mode = MULTIPLE_BUTTONS;
    init();
    INSERT_INTO_LINKED_LIST(this, first, last);
  }
  template <size_t M, size_t N>
  BankSelector(Bank &bank, const pin_t (&switchPins)[M], const pin_t (&ledPins)[N]) // One or multiple buttons, multiple LEDs
      : bank(bank), switchPins(switchPins), ledPins(ledPins), nb_settings(N)
  {
    if (M == 1)
    {
      dbButton1.pin = this->switchPins[0];
      mode = INCREMENT_LEDS;
    }
    else if (M == 2)
    {
      dbButton1.pin = this->switchPins[0];
      dbButton2.pin = this->switchPins[1];
      mode = INCREMENT_DECREMENT_LEDS;
    }
    else
    {
      mode = MULTIPLE_BUTTONS_LEDS;
      nb_settings = N < M ? N : M; // min(N, M)
    }
    init();
    INSERT_INTO_LINKED_LIST(this, first, last);
  }
  BankSelector(Bank &bank, std::initializer_list<pin_t> switchPins, std::initializer_list<pin_t> ledPins) // One or multiple buttons, multiple LEDs
      : bank(bank), nb_settings(ledPins.size())
  {
    switchPinsStorage = (pin_t *)malloc(sizeof(pin_t) * switchPins.size());
    memcpy(switchPinsStorage, switchPins.begin(), sizeof(pin_t) * switchPins.size());
    this->switchPins = switchPinsStorage;
    ledPinsStorage = (pin_t *)malloc(sizeof(pin_t) * ledPins.size());
    memcpy(ledPinsStorage, ledPins.begin(), sizeof(pin_t) * ledPins.size());
    this->ledPins = ledPinsStorage;

    if (switchPins.size() == 1)
    {
      dbButton1.pin = this->switchPins[0];
      mode = INCREMENT_LEDS;
    }
    else if (switchPins.size() == 2)
    {
      dbButton1.pin = this->switchPins[0];
      dbButton2.pin = this->switchPins[1];
      mode = INCREMENT_DECREMENT_LEDS;
    }
    else
    {
      mode = MULTIPLE_BUTTONS_LEDS;
      nb_settings = switchPins.size() < ledPins.size() ? switchPins.size() : ledPins.size(); // min(ledPins.size(), switchPins.size())
    }
    init();
    INSERT_INTO_LINKED_LIST(this, first, last);
  }
  BankSelector(Bank &bank, const pin_t (&switchPins)[2], pin_t nb_settings) // Two buttons (+1, -1), no LEDs
      : bank(bank), switchPins(switchPins), nb_settings(nb_settings)
  {
    dbButton1.pin = switchPins[0];
    dbButton2.pin = switchPins[1];
    mode = INCREMENT_DECREMENT;
    init();
    INSERT_INTO_LINKED_LIST(this, first, last);
  }
  BankSelector(Bank &bank, const pin_t (&switchPins)[1], pin_t nb_settings) // One button (+1), no LEDs
      : bank(bank), switchPins(switchPins), nb_settings(nb_settings)
  {
    dbButton1.pin = switchPins[0];
    mode = INCREMENT;
    init();
    INSERT_INTO_LINKED_LIST(this, first, last);
  }
  BankSelector(Bank &bank, std::initializer_list<pin_t> switchPins, pin_t nb_settings) // One or two buttons (+1, (-1)), no LEDs
      : bank(bank), nb_settings(nb_settings)
  {
    switchPinsStorage = (pin_t *)malloc(sizeof(pin_t) * switchPins.size());
    memcpy(switchPinsStorage, switchPins.begin(), sizeof(pin_t) * switchPins.size());
    this->switchPins = switchPinsStorage;
    if (switchPins.size() == 1)
    {
      dbButton1.pin = this->switchPins[0];
      mode = INCREMENT;
    }
    else if (switchPins.size() == 2)
    {
      dbButton1.pin = this->switchPins[0];
      dbButton2.pin = this->switchPins[1];
      mode = INCREMENT_DECREMENT;
    }
    init();
    INSERT_INTO_LINKED_LIST(this, first, last);
  }

  ~BankSelector()
  {
    free(ledPinsStorage);
    free(switchPinsStorage);
    if (previous != nullptr)
      previous->next = next;
    if (this == last)
      last = previous;
    if (next != nullptr)
      next->previous = previous;
    if (this == first)
      first = next;
  }

  void init();
  void refresh();

  uint8_t getBankSetting();
  void setBankSetting(uint8_t newBankSetting);

  const char *getMode();

  void setBankSettingChangeEvent(void (*fn)(uint8_t));

  BankSelector *getNext()
  {
    return next;
  }
  static BankSelector *getFirst()
  {
    return first;
  }

private:
  Bank &bank;
  uint8_t bankSetting = 0;
  pin_t switchPin, ledPin;
  const pin_t *switchPins, *ledPins;
  pin_t *switchPinsStorage = nullptr, *ledPinsStorage = nullptr;
  uint8_t nb_settings;

  unsigned long prevBounceTime = 0;

  struct debouncedButton
  {
    pin_t pin;
    bool prevState = HIGH;
  } dbButton1, dbButton2;

  const unsigned long debounceTime = 25;

  const int8_t falling = LOW - HIGH;
  const int8_t rising = HIGH - LOW;

  void (*bankSettingChangeEvent)(uint8_t) = nullptr;

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

  void refreshLEDs(uint8_t newBankSetting);
  bool debounceButton(debouncedButton &button);

  BankSelector *next = nullptr, *previous = nullptr;

  static BankSelector *last;
  static BankSelector *first;
};

#endif // BANKSELECTOR_H_