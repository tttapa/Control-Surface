/*
  This is an example of the "Bank" class of the MIDI_controller library.
  Connect two potentiometers to analog pins A0 and A1,
  and two pushbuttons to pins 2 and 3.

  Connect push buttons to pins 11 and 12, and 4 LEDs (+ current limiting resitors) to pins 4, 5, 6 and 7).

  When bank 1 is selected:
    Potentiometer A is channel volume of track 1      (Controller number 0x07, MIDI channel 1)
    Potentiometer B is channel volume of track 2      (Controller number 0x07, MIDI channel 2)
    Mute button A is the mute button for track 1      (Note number 0x10, MIDI channel 1)
    Mute button B is the mute button for track 2      (Note number 0x11, MIDI channel 1)
    The LED on pin 4 lights up.

  When bank 2 is selected:
    Potentiometer A is channel volume of track 3      (Controller number 0x07, MIDI channel 3)
    Potentiometer B is channel volume of track 4      (Controller number 0x07, MIDI channel 4)
    Mute button A is the mute button for track 3      (Note number 0x12, MIDI channel 1)
    Mute button B is the mute button for track 4      (Note number 0x13, MIDI channel 1)
    The LED on pin 5 lights up.

  When bank 3 is selected:
    Potentiometer A is channel volume of track 5      (Controller number 0x07, MIDI channel 5)
    Potentiometer B is channel volume of track 6      (Controller number 0x07, MIDI channel 6)
    Mute button A is the mute button for track 5      (Note number 0x14, MIDI channel 1)
    Mute button B is the mute button for track 6      (Note number 0x15, MIDI channel 1)
    The LED on pin 6 lights up.

  When bank 4 is selected:
    Potentiometer A is channel volume of track 7      (Controller number 0x07, MIDI channel 7)
    Potentiometer B is channel volume of track 8      (Controller number 0x07, MIDI channel 8)
    Mute button A is the mute button for track 7      (Note number 0x16, MIDI channel 1)
    Mute button B is the mute button for track 8      (Note number 0x17, MIDI channel 1)
    The LED on pin 7 lights up.

  This allows you to control multiple tracks with only a limited amount of physical potentiometers and buttons

  Map accordingly in your DAW or DJ software.

  Written by Pieter P, 08-09-2017
  https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_Controller.h> // Include the library

// Create a two new instances of the class 'Analog', on pins A0 and A1,
// that send MIDI messages with controller 7 (channel volume) on channels 1 and 2
Analog potentiometer_A(A0, MIDI_CC::Channel_Volume, 1);
Analog potentiometer_B(A1, MIDI_CC::Channel_Volume, 2);

// Create a two new instances of the class 'Digital', on pins 2 and 3,
// that send MIDI messages with note numbers 0x10 and 0x11 on MIDI channel 1
Digital muteButton_A(2, 0x10, 1);
Digital muteButton_B(3, 0x11, 1);

// Create a new bank that has two tracks per bank
Bank bank(2);

// Create a new bank selector that changes the bank setting of the bank we just created
// It has pushbuttons connected to pins 11 and 12 that increment or decrement the bank setting,
// and 4 LEDs to pins 4, 5, 6 and 7 that display the current bank setting.
BankSelector bankSelector(bank, { 11, 12 }, { 4, 5, 6, 7 } );

/* Alternatively, you can use arrays for the pin numbers:

   const pin_t buttonPins[] = { 11, 12 };
   const pin_t ledPins[] = { 4, 5, 6, 7 };

   BankSelector bankSelector(bank, buttonPins, ledPins);
*/

/*_______________________________________________________________________________________________________________________________________*/

void setup() {
  // Add the created objects to the bank
  bank.add(potentiometer_A, Bank::CHANGE_CHANNEL); // When the bank setting is changed, change the channel of the potentiometer
  bank.add(potentiometer_B, Bank::CHANGE_CHANNEL);
  bank.add(muteButton_A, Bank::CHANGE_ADDRESS); // When the bank setting is changed, change the address (note number) of the mute button
  bank.add(muteButton_B, Bank::CHANGE_ADDRESS);
}

/*_______________________________________________________________________________________________________________________________________*/

void loop() {
  // Refresh the MIDI controller (check whether the inputs have changed since last time, if so, send the new value over MIDI)
  // It also refreshes the bank selector
  MIDI_Controller.refresh();
}

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
