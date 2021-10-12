// interrupt pins for known boards
#include <AH/Arduino-Wrapper.h>
#ifdef ARDUINO_ARCH_NRF52840
#include <pins_arduino.h>
#endif

// Teensy (and maybe others) define these automatically
#if !defined(CORE_NUM_INTERRUPT)

// Wiring boards
#if defined(WIRING)
  #define CORE_NUM_INTERRUPT	NUM_EXTERNAL_INTERRUPTS
  #if NUM_EXTERNAL_INTERRUPTS > 0
  #define CORE_INT0_PIN		EI0
  #endif
  #if NUM_EXTERNAL_INTERRUPTS > 1
  #define CORE_INT1_PIN		EI1
  #endif
  #if NUM_EXTERNAL_INTERRUPTS > 2
  #define CORE_INT2_PIN		EI2
  #endif
  #if NUM_EXTERNAL_INTERRUPTS > 3
  #define CORE_INT3_PIN		EI3
  #endif
  #if NUM_EXTERNAL_INTERRUPTS > 4
  #define CORE_INT4_PIN		EI4
  #endif
  #if NUM_EXTERNAL_INTERRUPTS > 5
  #define CORE_INT5_PIN		EI5
  #endif
  #if NUM_EXTERNAL_INTERRUPTS > 6
  #define CORE_INT6_PIN		EI6
  #endif
  #if NUM_EXTERNAL_INTERRUPTS > 7
  #define CORE_INT7_PIN		EI7
  #endif

// Arduino Uno, Duemilanove, Diecimila, LilyPad, Mini, Fio, etc...
#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PB__) ||defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__)
  #define CORE_NUM_INTERRUPT	2
  #define CORE_INT0_PIN		2
  #define CORE_INT1_PIN		3

// Arduino Mega
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  #define CORE_NUM_INTERRUPT	6
  #define CORE_INT0_PIN		2
  #define CORE_INT1_PIN		3
  #define CORE_INT2_PIN		21
  #define CORE_INT3_PIN		20
  #define CORE_INT4_PIN		19
  #define CORE_INT5_PIN		18

// Arduino Nano Every, Uno R2 Wifi
#elif defined(__AVR_ATmega4809__)
  #define CORE_NUM_INTERRUPT  22
  #define CORE_INT0_PIN   0
  #define CORE_INT1_PIN   1
  #define CORE_INT2_PIN   2
  #define CORE_INT3_PIN   3
  #define CORE_INT4_PIN   4
  #define CORE_INT5_PIN   5
  #define CORE_INT6_PIN   6
  #define CORE_INT7_PIN   7
  #define CORE_INT8_PIN   8
  #define CORE_INT9_PIN   9
  #define CORE_INT10_PIN  10
  #define CORE_INT11_PIN  11
  #define CORE_INT12_PIN  12
  #define CORE_INT13_PIN  13
  #define CORE_INT14_PIN  14
  #define CORE_INT15_PIN  15
  #define CORE_INT16_PIN  16
  #define CORE_INT17_PIN  17
  #define CORE_INT18_PIN  18
  #define CORE_INT19_PIN  19
  #define CORE_INT20_PIN  20
  #define CORE_INT21_PIN  21

// Arduino Leonardo (untested)
#elif defined(__AVR_ATmega32U4__) && !defined(CORE_TEENSY)
  #define CORE_NUM_INTERRUPT	5
  #define CORE_INT0_PIN		3
  #define CORE_INT1_PIN		2
  #define CORE_INT2_PIN		0
  #define CORE_INT3_PIN		1
  #define CORE_INT4_PIN		7

// Sanguino (untested) and ATmega1284P
#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega1284P__)
  #define CORE_NUM_INTERRUPT	3
  #define CORE_INT0_PIN		10
  #define CORE_INT1_PIN		11
  #define CORE_INT2_PIN		2

// ATmega32u2 and ATmega32u16 based boards with HoodLoader2
#elif defined(__AVR_ATmega32U2__) || defined(__AVR_ATmega16U2__)
  #define CORE_NUM_INTERRUPT 8
  #define CORE_INT0_PIN 8
  #define CORE_INT1_PIN 17
  #define CORE_INT2_PIN 13
  #define CORE_INT3_PIN 14
  #define CORE_INT4_PIN 15
  #define CORE_INT5_PIN 16
  #define CORE_INT6_PIN 19
  #define CORE_INT7_PIN 20

// Chipkit Uno32 - attachInterrupt may not support CHANGE option
#elif defined(__PIC32MX__) && defined(_BOARD_UNO_)
  #define CORE_NUM_INTERRUPT	5
  #define CORE_INT0_PIN		38
  #define CORE_INT1_PIN		2
  #define CORE_INT2_PIN		7
  #define CORE_INT3_PIN		8
  #define CORE_INT4_PIN		35

// Chipkit Uno32 - attachInterrupt may not support CHANGE option
#elif defined(__PIC32MX__) && defined(_BOARD_MEGA_)
  #define CORE_NUM_INTERRUPT	5
  #define CORE_INT0_PIN		3
  #define CORE_INT1_PIN		2
  #define CORE_INT2_PIN		7
  #define CORE_INT3_PIN		21
  #define CORE_INT4_PIN		20

// http://hlt.media.mit.edu/?p=1229
#elif defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  #define CORE_NUM_INTERRUPT    1
  #define CORE_INT0_PIN		2

// ATtiny441 ATtiny841
#elif defined(__AVR_ATtiny441__) || defined(__AVR_ATtiny841__)
  #define CORE_NUM_INTERRUPT	1
  #define CORE_INT0_PIN		9

//https://github.com/SpenceKonde/ATTinyCore/blob/master/avr/extras/ATtiny_x313.md
#elif defined(__AVR_ATtinyX313__)
  #define CORE_NUM_INTERRUPT    2
  #define CORE_INT0_PIN		4
  #define CORE_INT1_PIN		5
 
// Attiny167 same core as abobe
#elif defined(__AVR_ATtiny167__)
  #define CORE_NUM_INTERRUPT	2
  #define CORE_INT0_PIN		14
  #define CORE_INT1_PIN		3

  
// Arduino Due
#elif defined(__SAM3X8E__) 
  #define CORE_NUM_INTERRUPT	54
  #define CORE_INT0_PIN		0
  #define CORE_INT1_PIN		1
  #define CORE_INT2_PIN		2
  #define CORE_INT3_PIN		3
  #define CORE_INT4_PIN		4
  #define CORE_INT5_PIN		5
  #define CORE_INT6_PIN		6
  #define CORE_INT7_PIN		7
  #define CORE_INT8_PIN		8
  #define CORE_INT9_PIN		9
  #define CORE_INT10_PIN	10
  #define CORE_INT11_PIN	11
  #define CORE_INT12_PIN	12
  #define CORE_INT13_PIN	13
  #define CORE_INT14_PIN	14
  #define CORE_INT15_PIN	15
  #define CORE_INT16_PIN	16
  #define CORE_INT17_PIN	17
  #define CORE_INT18_PIN	18
  #define CORE_INT19_PIN	19
  #define CORE_INT20_PIN	20
  #define CORE_INT21_PIN	21
  #define CORE_INT22_PIN	22
  #define CORE_INT23_PIN	23
  #define CORE_INT24_PIN	24
  #define CORE_INT25_PIN	25
  #define CORE_INT26_PIN	26
  #define CORE_INT27_PIN	27
  #define CORE_INT28_PIN	28
  #define CORE_INT29_PIN	29
  #define CORE_INT30_PIN	30
  #define CORE_INT31_PIN	31
  #define CORE_INT32_PIN	32
  #define CORE_INT33_PIN	33
  #define CORE_INT34_PIN	34
  #define CORE_INT35_PIN	35
  #define CORE_INT36_PIN	36
  #define CORE_INT37_PIN	37
  #define CORE_INT38_PIN	38
  #define CORE_INT39_PIN	39
  #define CORE_INT40_PIN	40
  #define CORE_INT41_PIN	41
  #define CORE_INT42_PIN	42
  #define CORE_INT43_PIN	43
  #define CORE_INT44_PIN	44
  #define CORE_INT45_PIN	45
  #define CORE_INT46_PIN	46
  #define CORE_INT47_PIN	47
  #define CORE_INT48_PIN	48
  #define CORE_INT49_PIN	49
  #define CORE_INT50_PIN	50
  #define CORE_INT51_PIN	51
  #define CORE_INT52_PIN	52
  #define CORE_INT53_PIN	53

// ESP8266 (https://github.com/esp8266/Arduino/)
#elif defined(ESP8266)
  #define CORE_NUM_INTERRUPT EXTERNAL_NUM_INTERRUPTS
  #define CORE_INT0_PIN		0
  #define CORE_INT1_PIN		1
  #define CORE_INT2_PIN		2
  #define CORE_INT3_PIN		3
  #define CORE_INT4_PIN		4
  #define CORE_INT5_PIN		5
  // GPIO6-GPIO11 are typically used to interface with the flash memory IC on 
  // most esp8266 modules, so we should avoid adding interrupts to these pins.
  #define CORE_INT12_PIN	12
  #define CORE_INT13_PIN	13
  #define CORE_INT14_PIN	14
  #define CORE_INT15_PIN	15

// ESP32 (https://github.com/espressif/arduino-esp32)
#elif defined(ESP32)

  #define CORE_NUM_INTERRUPT  40 
  #define CORE_INT0_PIN		0
  #define CORE_INT1_PIN		1
  #define CORE_INT2_PIN		2
  #define CORE_INT3_PIN		3
  #define CORE_INT4_PIN		4
  #define CORE_INT5_PIN		5
  // GPIO6-GPIO11 are typically used to interface with the flash memory IC on 
  // esp32, so we should avoid adding interrupts to these pins.
  #define CORE_INT12_PIN	12
  #define CORE_INT13_PIN	13
  #define CORE_INT14_PIN	14
  #define CORE_INT15_PIN	15
  #define CORE_INT16_PIN        16
  #define CORE_INT17_PIN        17
  #define CORE_INT18_PIN        18
  #define CORE_INT19_PIN        19
  #define CORE_INT21_PIN        21
  #define CORE_INT22_PIN        22
  #define CORE_INT23_PIN        23
  #define CORE_INT25_PIN        25
  #define CORE_INT26_PIN        26
  #define CORE_INT27_PIN        27
  #define CORE_INT32_PIN        32
  #define CORE_INT33_PIN        33
  #define CORE_INT34_PIN        34
  #define CORE_INT35_PIN        35
  #define CORE_INT36_PIN        36
  #define CORE_INT39_PIN        39


// Arduino Zero - TODO: interrupts do not seem to work
//                      please help, contribute a fix!
#elif defined(__SAMD21G18A__)
  #define CORE_NUM_INTERRUPT	31
  #define CORE_INT0_PIN		0
  #define CORE_INT1_PIN		1
  #define CORE_INT2_PIN		2
  #define CORE_INT3_PIN		3
  #define CORE_INT4_PIN		4
  #define CORE_INT5_PIN		5
  #define CORE_INT6_PIN		6
  #define CORE_INT7_PIN		7
  #define CORE_INT8_PIN		8
  #define CORE_INT9_PIN		9
  #define CORE_INT10_PIN	10
  #define CORE_INT11_PIN	11
  #define CORE_INT12_PIN	12
  #define CORE_INT13_PIN	13
  #define CORE_INT14_PIN	14
  #define CORE_INT15_PIN	15
  #define CORE_INT16_PIN	16
  #define CORE_INT17_PIN	17
  #define CORE_INT18_PIN	18
  #define CORE_INT19_PIN	19
  #define CORE_INT20_PIN	20
  #define CORE_INT21_PIN	21
  #define CORE_INT22_PIN	22
  #define CORE_INT23_PIN	23
  #define CORE_INT24_PIN	24
  #define CORE_INT25_PIN	25
  #define CORE_INT26_PIN	26
  #define CORE_INT27_PIN	27
  #define CORE_INT28_PIN	28
  #define CORE_INT29_PIN	29
  #define CORE_INT30_PIN	30

#elif defined(__SAMD51__)
  #define CORE_NUM_INTERRUPT	26
  #define CORE_INT0_PIN		0
  #define CORE_INT1_PIN		1
  #define CORE_INT2_PIN		2
  #define CORE_INT3_PIN		3
  #define CORE_INT4_PIN		4
  #define CORE_INT5_PIN		5
  #define CORE_INT6_PIN		6
  #define CORE_INT7_PIN		7
  #define CORE_INT8_PIN		8
  #define CORE_INT9_PIN		9
  #define CORE_INT10_PIN	10
  #define CORE_INT11_PIN	11
  #define CORE_INT12_PIN	12
  #define CORE_INT13_PIN	13
  #define CORE_INT14_PIN	14
  #define CORE_INT15_PIN	15
  #define CORE_INT16_PIN	16
  #define CORE_INT17_PIN	17
  #define CORE_INT18_PIN	18
  #define CORE_INT19_PIN	19
  #define CORE_INT20_PIN	20
  #define CORE_INT21_PIN	21
  #define CORE_INT22_PIN	22
  #define CORE_INT23_PIN	23
  #define CORE_INT24_PIN	24
  #define CORE_INT25_PIN	25

// Arduino 101
#elif defined(__arc__)
  #define CORE_NUM_INTERRUPT	14
  #define CORE_INT2_PIN		2
  #define CORE_INT5_PIN		5
  #define CORE_INT7_PIN		7
  #define CORE_INT8_PIN		8
  #define CORE_INT10_PIN	10
  #define CORE_INT11_PIN	11
  #define CORE_INT12_PIN	12
  #define CORE_INT13_PIN	13

// Arduino Nano BLE
#elif defined(ARDUINO_ARCH_NRF52840)
  #define CORE_NUM_INTERRUPT	NUM_DIGITAL_PINS

#endif
#endif

#if !defined(CORE_NUM_INTERRUPT)
#error "Interrupts are unknown for this board, please add to this code"
#endif
#if CORE_NUM_INTERRUPT <= 0
#error "Encoder requires interrupt pins, but this board does not have any :("
#error "You could try defining ENCODER_DO_NOT_USE_INTERRUPTS as a kludge."
#endif
