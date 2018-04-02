#ifndef SELECTOR_H_
#define SELECTOR_H_

#include <string.h>

#include "../Helpers/Linked_List.h"
#include "../Helpers/Copy.hpp"
#include "../ExtendedInputOutput/ExtendedInputOutput.h"
#include "../Settings/SettingsWrapper.h"

using namespace ExtIO;

class Selector
{
public:
  enum buttonType
  {
    TOGGLE,
    MOMENTARY
  };

  /*
  Different  Select modes:

  - One toggle switch (latching switch)

      When the switch is in the 'off' position, Setting 1 is selected
      When the switch is in the 'on' position, Setting 2 is selected

      Selector(, switch pin, Selector::TOGGLE);


  - One toggle switch (latching switch) and one LED

      When the switch is in the 'off' position, Setting 1 is selected and the LED is off
      When the switch is in the 'on' position, Setting 2 is selected and the LED is on

          Note: this mode is pretty useless, you can just connect the LED to the switch directly,
                without wasting a digital output pin on it.

      Selector(, switch pin, led pin, Selector::TOGGLE);


  - One momentary switch (push button)

      Pressing the button switches the Setting:
      When starting the program, Setting 1 is selected,
      When the button is pressed, Setting 2 is selected,
      When the button is pressed again, Setting 1 is selected, 
      and so on.

      Selector(, button pin);
      Selector(, button pin, Selector::MOMENTARY);


  - One momentary switch (push button) and one LED

      Pressing the button switches the Setting and toggles the LED:
      When starting the program, Setting 1 is selected and the LED is off,
      When the button is pressed, Setting 2 is selected and the LED turns on,
      When the button is pressed again, Setting 1 is selected and the LED is turned off,
      and so on.

      Selector(, button pin, led pin);
      Selector(, button pin, led pin, Selector::MOMENTARY);


  - Multiple momentary switches (push buttons)

      Pressing one of the buttons selects the respective output:
      When starting the program, Setting 1 is selected,
      When the second button is pressed, Setting 2 is selected,
      When the n-th button is pressed, Setting n is selected.

      Selector(, { button 1 pin, button 2 pin, ... , button n pin } );


  - Multiple momentary switches (push buttons) and multiple LEDs

      Pressing one of the buttons selects the respective output and enables the respective LED:
      When starting the program, Setting 1 is selected and LED 1 is on,
      When the second button is pressed, Setting 2 is selected, LED 1 turns off and LED 2 turns on,
      When the n-th button is pressed, Setting n is selected, LED n turns on, and all other LEDs are off.

      Selector(, { button 1 pin, button 2 pin, ... , button n pin }, { led 1 pin, led 2 pin, ... , led n pin } );

      
  - Two momentary switches (push buttons)

      Pressing the first button increments the Setting number,
      pressing the second button decrements the Setting number: 
      When starting the program, Setting 1 is selected,
      When the first button is pressed, Setting 2 is selected, 
      When the first button is pressed again, Setting 3 is selected,
      When the last Setting is selected, and the first button is pressed again,
      Setting 1 is selected.
      When the second button is pressed, the last Setting (n) is selected,
      When the second button is pressed again, Setting (n-1) is selected,
      and so on.

      Selector(, { button increment pin, button decrement pin }, number of Settings);


  - Two momentary switches (push buttons) and multiple LEDs

      Pressing the first button increments the Setting number and turns on the respective LED,
      pressing the second button decrements the Setting number and turns on the respective LED: 
      When starting the program, Setting 1 is selected and LED 1 is on,
      When the first button is pressed, Setting 2 is selected, LED 1 turns off and LED 2 turns on,
      When the first button is pressed again, Setting 3 is selected, LED 2 turns off and LED 3 turns on.
      When the last Setting is selected, and the first button is pressed,
      Setting 1 is selected, the last LED turns off and LED 1 turns on.
      When the second button is pressed, the last Setting (n) is selected, LED 1 turns off and LED n turns on,
      When the second button is pressed again, Setting (n-1) is selected, LED n turns off and LED n-1 turns on,
      and so on.

      Selector(, { button increment pin, button decrement pin }, { led 1 pin, led 2 pin, ... , led n pin });


  - One momentary switch (push button)

      Pressing the button increments the Setting number,
      When starting the program, Setting 1 is selected,
      When the button is pressed, Setting 2 is selected, 
      When the button is pressed again, Setting 3 is selected,
      When the last Setting is selected, and the button is pressed again,
      Setting 1 is selected.

      Selector(, { button increment pin }, number of Settings);


  - One momentary switch (push button) and multiple LEDs

      Pressing the button increments the Setting number and turns on the respective LED,
      When starting the program, Setting 1 is selected and LED 1 is on,
      When the button is pressed, Setting 2 is selected, LED 1 turns off and LED 2 turns on,
      When the button is pressed again, Setting 3 is selected, LED 2 turns off and LED 3 turns on.
      When the last Setting is selected, and the button is pressed,
      Setting 1 is selected, the last LED turns off and LED 1 turns on.
      
      Selector(, { button increment pin }, { led 1 pin, led 2 pin, ... , led n pin });


    Note: a switch is 'off' or 'released' when it doesn't conduct. The digital value 
    on the input will therefore be HIGH (because of the pull-up resistor)
  */

  Selector(pin_t switchPin, buttonType buttonType = MOMENTARY) // One switch or button, no LEDs
      : nb_settings(2)
  {
    dbButton1.pin = switchPin;
    if (buttonType == TOGGLE)
      mode = SINGLE_SWITCH;
    else
      mode = SINGLE_BUTTON;
    pinMode(switchPin, INPUT_PULLUP);
    LinkedList::append(this, first, last);
  }

  Selector(pin_t switchPin, pin_t ledPin, buttonType buttonType = MOMENTARY) // One switch or button, one LED
      : ledPin(ledPin), nb_settings(2)
  {
    dbButton1.pin = switchPin;
    if (buttonType == TOGGLE)
      mode = SINGLE_SWITCH_LED;
    else
      mode = SINGLE_BUTTON_LED;
    pinMode(switchPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    LinkedList::append(this, first, last);
  }

  template <size_t N>
  Selector(const pin_t (&switchPins)[N]) // Multiple buttons, no LEDs
      : nb_settings(N)
  {
    static_assert(N > 1, "Error: minimum number of switches is 2. ");
    this->switchPins = new pin_t[N];
    copy(this->switchPins, switchPins);
    mode = MULTIPLE_BUTTONS;
    for (uint8_t i = 0; i < nb_settings; i++)
      pinMode(this->switchPins[i], INPUT_PULLUP);
    LinkedList::append(this, first, last);
  }

  template <size_t N, size_t M>
  Selector(const pin_t (&switchPins)[N], const pin_t (&ledPins)[M]) // One or multiple buttons, multiple LEDs
      : nb_settings(M)
  {
    static_assert(N <= 2 || N == M, "Error: number of LEDs should be the same as the number of buttons. ");

    this->ledPins = new pin_t[M];
    copy(this->ledPins, ledPins);
    if (N == 1)
    {
      dbButton1.pin = switchPins[0];
      mode = INCREMENT_LEDS;
      pinMode(dbButton1.pin, INPUT_PULLUP);
    }
    else if (N == 2)
    {
      dbButton1.pin = switchPins[0];
      dbButton2.pin = switchPins[1];
      mode = INCREMENT_DECREMENT_LEDS;
      pinMode(dbButton1.pin, INPUT_PULLUP);
      pinMode(dbButton2.pin, INPUT_PULLUP);
    }
    else
    {
      this->switchPins = new pin_t[N];
      copy(this->switchPins, switchPins);
      mode = MULTIPLE_BUTTONS_LEDS;
      for (uint8_t i = 0; i < nb_settings; i++)
        pinMode(switchPins[i], INPUT_PULLUP);
    }
    initLEDs();
    LinkedList::append(this, first, last);
  }
  template <size_t N>
  Selector(const pin_t (&switchPins)[N], pin_t nb_settings) // Two buttons (+1, -1), no LEDs
      : nb_settings(nb_settings)
  {
    static_assert(N <= 2, "Error: maximum number of buttons in the increment/decrement configuration is 2. ");
    if (N == 1)
    {
      dbButton1.pin = switchPins[0];
      mode = INCREMENT;
      pinMode(dbButton1.pin, INPUT_PULLUP);
      LinkedList::append(this, first, last);
    }
    else if (N == 2)
    {
      dbButton1.pin = switchPins[0];
      dbButton2.pin = switchPins[1];
      mode = INCREMENT_DECREMENT;
      pinMode(dbButton1.pin, INPUT_PULLUP);
      pinMode(dbButton2.pin, INPUT_PULLUP);
      LinkedList::append(this, first, last);
    }
  }

  ~Selector()
  {
    delete[] switchPins;
    delete[] ledPins;
    LinkedList::remove(this, first, last);
  }
  void initLEDs()
  {
    for (uint8_t i = 0; i < nb_settings; i++)
      pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[0], HIGH);
  }

  void refresh();

  uint8_t getSetting();
  void setSetting(uint8_t newSetting);

  // #ifdef DEBUG
  const char *getMode();
  // #endif

  Selector *getNext()
  {
    return next;
  }
  static Selector *getFirst()
  {
    return first;
  }

private:
  pin_t switchPin = 0;
  pin_t ledPin = 0;
  pin_t *switchPins = nullptr;
  pin_t *ledPins = nullptr;

  bool firstRefresh = true;

  unsigned long prevBounceTime = 0;

  struct debouncedButton
  {
    pin_t pin;
    bool prevState = HIGH;
  } dbButton1, dbButton2;

  const unsigned long debounceTime = BUTTON_DEBOUNCE_TIME; // Edit this in ../Settings/Settings.h

  const static int8_t falling = LOW - HIGH;
  const static int8_t rising = HIGH - LOW;

  enum SelectorMode
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

  void refreshLEDs(uint8_t newSetting);
  bool debounceButton(debouncedButton &button);

  Selector *next = nullptr, *previous = nullptr;
  static Selector *last;
  static Selector *first;
  template <class Node>
  friend void LinkedList::append(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::moveDown(Node *, Node *&, Node *&);
  template <class Node>
  friend void LinkedList::remove(Node *, Node *&, Node *&);

protected:
  uint8_t Setting = 0;
  uint8_t nb_settings = 1;
  virtual void refreshImpl(uint8_t newSetting) {}
  };

#endif // SELECTOR_H_