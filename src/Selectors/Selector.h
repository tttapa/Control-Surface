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
  /**
 * @brief The type of switch the Selector will be used with.
 * 
 */
  enum SwitchType
  {
    /**
     * @brief A toggle switch that remains in the set position.   
     */
    TOGGLE,
    /**
     * @brief A momentary push button that returns to the 'off'
     *        position when released
     */
    MOMENTARY
  };

/**
 * @brief Construct a Selector object with one switch.

 * <h3>One toggle switch (latching switch)</h3>
 * 
 * When the switch is in the 'off' position, Setting 1 is selected  
 * When the switch is in the 'on' position, Setting 2 is selected
 *
 * Selector ( switchPin, Selector::TOGGLE );

 * <h3>One momentary switch (push button)</h3>
 *
 * Pressing the button switches the Setting:  
 * When starting the program, Setting 1 is selected,  
 * When the button is pressed, Setting 2 is selected,  
 * When the button is pressed again, Setting 1 is selected,  
 * and so on.
 *
 * Selector ( buttonPin );  
 * Selector ( buttonPin, Selector::MOMENTARY );
 
 * @param switchPin The pin with the switch connected. The built-in pull-up resistor will be enabled.
 * @param switchType The type of switch. (See Selector::SwitchType)
 */
  Selector(pin_t switchPin, SwitchType switchType = MOMENTARY) // One switch or button, no LEDs
      : nb_settings(2)
  {
    dbButton1.pin = switchPin;
    if (switchType == TOGGLE)
      mode = SINGLE_SWITCH;
    else
      mode = SINGLE_BUTTON;
    pinMode(switchPin, INPUT_PULLUP);
    LinkedList::append(this, first, last);
  }
/**
 * @brief Construct a Selector object with one switch and one LED.
 
 * <h3>One toggle switch (latching switch) and one LED</h3>
 *
 * When the switch is in the 'off' position, Setting 1 is selected and the LED is off  
 * When the switch is in the 'on' position, Setting 2 is selected and the LED is on
 *
 * \note This mode is pretty useless, you can just connect the LED to the switch directly,
 *       without wasting a digital output pin on it.
 *
 * Selector ( pin_t switchPin, pin_t ledPin, Selector::TOGGLE );

 * <h3>One momentary switch (push button) and one LED</h3>
 *
 * Pressing the button switches the Setting and toggles the LED:  
 * When starting the program, Setting 1 is selected and the LED is off,  
 * When the button is pressed, Setting 2 is selected and the LED turns on,  
 * When the button is pressed again, Setting 1 is selected and the LED is turned off,  
 * and so on.
 *
 * Selector ( buttonPin, ledPin );  
 * Selector ( buttonPin, ledPin, Selector::MOMENTARY );

 * @param switchPin The pin with the switch connected. The built-in pull-up resistor will be enabled.
 * @param ledPin The pin with the LED connected.
 * @param switchType The type of switch. (See Selector::SwitchType)
 */
  Selector(pin_t switchPin, pin_t ledPin, SwitchType switchType = MOMENTARY) // One switch or button, one LED
      : ledPin(ledPin), nb_settings(2)
  {
    dbButton1.pin = switchPin;
    if (switchType == TOGGLE)
      mode = SINGLE_SWITCH_LED;
    else
      mode = SINGLE_BUTTON_LED;
    pinMode(switchPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    LinkedList::append(this, first, last);
  }

/**
 * @brief Construct a Selector object with multiple buttons.

 * <h3>Multiple momentary switches (push buttons)</h3>
 *
 * Pressing one of the buttons selects the respective output:  
 * When starting the program, Setting 1 is selected,  
 * When the second button is pressed, Setting 2 is selected,  
 * When the N-th button is pressed, Setting N is selected.
 *
 * Selector( { buttonPin1, buttonPin2, ... , buttonPinN } );

 * @tparam N The number of buttons.
 * @param switchPins A list of pin numbers with the buttons connected.
 */
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
/**
 * @brief Construct a new Selector object

 * <h3>Multiple momentary switches (push buttons) and multiple LEDs</h3>
 *
 * Pressing one of the buttons selects the respective output and enables the respective LED:  
 * When starting the program, Setting 1 is selected and LED 1 is on,  
 * When the second button is pressed, Setting 2 is selected, LED 1 turns off and LED 2 turns on,  
 * When the N-th button is pressed, Setting N is selected, the N-th LED turns on, and all other LEDs are off.
 *
 * Selector( { buttonPin1, buttonPin2, ... , buttonPinN }, { ledPin1, ledPin2, ... , ledPinN } );

 * <h3>Two momentary switches (push buttons) and multiple LEDs (increment / decrement)</h3>
 *
 * Pressing the first button increments the Setting number and turns on the respective LED,  
 * pressing the second button decrements the Setting number and turns on the respective LED:  
 * When starting the program, Setting 1 is selected and LED 1 is on,  
 * When the first button is pressed, Setting 2 is selected, LED 1 turns off and LED 2 turns on,  
 * When the first button is pressed again, Setting 3 is selected, LED 2 turns off and LED 3 turns on.  
 * When the last Setting is selected, and the first button is pressed, 
 * Setting 1 is selected, the last LED turns off and LED 1 turns on.  
 * When the second button is pressed, the last Setting (M) is selected, LED 1 turns off and LED M turns on,  
 * When the second button is pressed again, Setting (M-1) is selected, LED M turns off and LED M-1 turns on,  
 * and so on.
 *
 * Selector( { incrementButtonPin, decrementButtonPin }, { ledPin1, ledPin2, ... , ledPinM });

 * <h3>One momentary switch (push button) and multiple LEDs (increment)</h3>
 *
 * Pressing the button increments the Setting number and turns on the respective LED,  
 * When starting the program, Setting 1 is selected and LED 1 is on,  
 * When the button is pressed, Setting 2 is selected, LED 1 turns off and LED 2 turns on,  
 * When the button is pressed again, Setting 3 is selected, LED 2 turns off and LED 3 turns on.  
 * When the M-th Setting is selected, and the button is pressed, 
 * Setting 1 is selected, the M-th LED turns off and LED 1 turns on.
 * 
 * Selector( { incrementButtonPin }, { ledPin1, ledPin2, ... , ledPinM });

 * @tparam N The number of buttons.
 * @tparam M The number of LEDs.
 * @param switchPins A list of pin numbers with the buttons connected.
 * @param ledPins A list of pin numbers with the LEDs connected.
 */
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

/**
 * @brief Construct a Selector object with increment / decrement buttons.

 * <h3>One momentary switch (push button) (increment)</h3>
 *
 * Pressing the button increments the Setting number,  
 * When starting the program, Setting 1 is selected,  
 * When the button is pressed, Setting 2 is selected,  
 * When the button is pressed again, Setting 3 is selected,  
 * When the last Setting is selected, and the button is pressed again, 
 * Setting 1 is selected.
 *
 * Selector( { incrementButtonPin }, nb_settings );
      
 * <h3>Two momentary switches (push buttons) (increment / decrement)</h3>
 *
 * Pressing the first button increments the Setting number,  
 * pressing the second button decrements the Setting number:  
 * When starting the program, Setting 1 is selected,  
 * When the first button is pressed, Setting 2 is selected,  
 * When the first button is pressed again, Setting 3 is selected,  
 * When the last Setting is selected, and the first button is pressed again, 
 * Setting 1 is selected.  
 * When the second button is pressed, the last Setting (nb_settings) is selected,  
 * When the second button is pressed again, the previous Setting (nb_settings-1) is selected,  
 * and so on.
 *
 * Selector ( { incrementButtonPin, decrementButtonPin }, nb_settings );

 * @tparam N The number of buttons [1, 2].
 * @param switchPins A list of pin numbers with the buttons connected.
 * @param nb_settings The number of settings to cycle through.
 */
  template <size_t N>
  Selector(const pin_t (&switchPins)[N], uint8_t nb_settings) // Two buttons (+1, -1), no LEDs
      : nb_settings(nb_settings)
  {
    static_assert(N > 0, "Error: no switch pins specified. ");
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