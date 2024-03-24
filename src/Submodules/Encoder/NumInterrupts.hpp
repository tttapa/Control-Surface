#pragma once

#include <AH/Arduino-Wrapper.h>
#include <Settings/NamespaceSettings.hpp>

#ifdef ARDUINO_ARCH_NRF52840
#include <pins_arduino.h>
#endif

BEGIN_CS_NAMESPACE

// Teensy (and maybe others)
#if defined(CORE_NUM_INTERRUPT)
// CORE_NUM_INTERRUPT already defined by core

// Wiring boards
#elif defined(WIRING)
#define CORE_NUM_INTERRUPT NUM_EXTERNAL_INTERRUPTS

// MightyCore, MiniCore, etc.
#elif defined(EXTERNAL_NUM_INTERRUPTS)
#define CORE_NUM_INTERRUPT EXTERNAL_NUM_INTERRUPTS

// Arduino Uno, Duemilanove, Diecimila, LilyPad, Mini, Fio, etc...
#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PB__) ||           \
    defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__)
#define CORE_NUM_INTERRUPT 2

// Arduino Mega
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define CORE_NUM_INTERRUPT 6

// Arduino Nano Every, Uno R2 Wifi
#elif defined(__AVR_ATmega4809__)
#define CORE_NUM_INTERRUPT 22

// Arduino Leonardo
#elif defined(__AVR_ATmega32U4__) && !defined(CORE_TEENSY)
#define CORE_NUM_INTERRUPT 5

// Sanguino (untested) and ATmega1284P
#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__) ||             \
    defined(__AVR_ATmega1284P__)
#define CORE_NUM_INTERRUPT 3

// ATmega32u2 and ATmega32u16 based boards with HoodLoader2
#elif defined(__AVR_ATmega32U2__) || defined(__AVR_ATmega16U2__)
#define CORE_NUM_INTERRUPT 8

#elif defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
#define CORE_NUM_INTERRUPT 1

// https://github.com/SpenceKonde/ATTinyCore/blob/master/avr/extras/ATtiny_x313.md
#elif defined(__AVR_ATtinyX313__)
#define CORE_NUM_INTERRUPT 2

// Attiny167 same core as above
#elif defined(__AVR_ATtiny167__)
#define CORE_NUM_INTERRUPT 2

// Arduino Due
#elif defined(__SAM3X8E__)
#define CORE_NUM_INTERRUPT 54

// ESP8266 (https://github.com/esp8266/Arduino/)
#elif defined(ESP8266)
#define CORE_NUM_INTERRUPT EXTERNAL_NUM_INTERRUPTS

// ESP32 (https://github.com/espressif/arduino-esp32)
#elif defined(ESP32)
#define CORE_NUM_INTERRUPT 40

// Arduino Zero - TODO: interrupts do not seem to work
//                      please help, contribute a fix!
#elif defined(__SAMD21G18A__) || defined(__SAMD21E18A__)
#define CORE_NUM_INTERRUPT 31

// SAMD51
#elif defined(__SAMD51__)
#define CORE_NUM_INTERRUPT 26

// Arduino Nano BLE
#elif defined(ARDUINO_ARCH_NRF52840)
#define CORE_NUM_INTERRUPT NUM_DIGITAL_PINS

// Arduino Nano RP2040 Connect
#elif defined(ARDUINO_NANO_RP2040_CONNECT)
#define CORE_NUM_INTERRUPT 20

#elif defined(ARDUINO_ARCH_RP2040)
#define CORE_NUM_INTERRUPT 32

// ARM mbed OS
#elif defined(ARDUINO_ARCH_MBED)
#define CORE_NUM_INTERRUPT NUM_DIGITAL_PINS

// Arduino UNO R4
#elif defined(ARDUINO_UNOR4_MINIMA) || defined(ARDUINO_UNOR4_WIFI)
#define CORE_NUM_INTERRUPT 13

#ifdef NOT_AN_INTERRUPT
#error                                                                         \
    "This version of the ArduinoCore-renesas is not supported. Please open an issue on GitHub: https://github.com/tttapa/Control-Surface/issues"
#endif
#define NOT_AN_INTERRUPT 255
constexpr pin_size_t pin_to_interrupt_index[] {
    0,                // GPIO 0  (P301)                          IRQ6
    1,                // GPIO 1  (P302)                     IRQ5
    2,                // GPIO 2  (P104)      IRQ1
    3,                // GPIO 3  (P105) IRQ0
    NOT_AN_INTERRUPT, // GPIO 4  (P106) -
    NOT_AN_INTERRUPT, // GPIO 5  (P107) -
    4,                // GPIO 6  (P111)                IRQ4
    NOT_AN_INTERRUPT, // GPIO 7  (P112) -
    5,                // GPIO 8  (P304)                                    IRQ9
    NOT_AN_INTERRUPT, // GPIO 9  (P303) -
    NOT_AN_INTERRUPT, // GPIO 10 (P103) -
    6,                // GPIO 11 (P411)                IRQ4
    7,                // GPIO 12 (P410)                     IRQ5
    NOT_AN_INTERRUPT, // GPIO 13 (P102) -
    NOT_AN_INTERRUPT, // GPIO 14 (P014) -
    8,                // GPIO 15 (P000)                          IRQ6
    9,                // GPIO 16 (P001)                               IRQ7
    10,               // GPIO 17 (P002)           IRQ2
    11,               // GPIO 18 (P101)      IRQ1
    12,               // GPIO 19 (P100)           IRQ2
};
inline pin_size_t digitalPinToInterrupt(pin_size_t pin) {
    if (pin_to_interrupt_index[pin] == NOT_AN_INTERRUPT)
        return NOT_AN_INTERRUPT;
    return ::digitalPinToInterrupt(pin);
}
inline pin_size_t interruptToIndex(pin_size_t interrupt) {
    return pin_to_interrupt_index[interrupt];
}
#define CS_CUSTOM_INTERRUPT_TO_INDEX 1

// Others
#else
#warning "Unknown board. Please specify the number of external interrupts."
#define CORE_NUM_INTERRUPT NUM_DIGITAL_PINS

#endif

END_CS_NAMESPACE
